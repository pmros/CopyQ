/*
    Copyright (c) 2012, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>

#include <QRegExp>
#include <QLabel>
#include <QTimer>

/**
 * Delegate for items in ClipboardBrowser.
 *
 * Creates editor on demand and draws contents of all items.
 *
 * To achieve better performance the first call to get sizeHint() value for
 * an item returns some default value (so it doesn't have to render all items).
 * First paint() call for some item will load that item into cache and notify
 * all listeners
 *
 * If dryPaint() is set to true paint() invocation only loads given item to
 * cache if necessary but nothing is rendered.
 */
class ItemDelegate : public QItemDelegate
{
    Q_OBJECT

    public:
        ItemDelegate(QWidget *parent = 0);

        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

        void setEditorData(QWidget *editor, const QModelIndex &index) const;

        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

        bool eventFilter(QObject *object, QEvent *event);

        /** Remove all cached items (cache is refreshed using paint()). */
        void invalidateCache() const;

        /** Set dry paint. */
        void setDryPaint(
                bool dryPaint
                //!< If true disable item rendering (only cache is updated).
                )
        {
            m_dryPaint = dryPaint;
        }

        /** Return dry paint. */
        bool dryPaint() const { return m_dryPaint; }

        /** Set regular expression for highlighting. */
        void setSearch(const QRegExp &re);

        /** Search highlight style. */
        void setSearchStyle(const QFont &font, const QPalette &palette);

        /** Editor widget style. */
        void setEditorStyle(const QFont &font, const QPalette &palette);

        /** Item number style. */
        void setNumberStyle(const QFont &font, const QPalette &palette);

        /** Show/hide item number. */
        void setShowNumber(bool show) { m_showNumber = show; }

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    private:
        QWidget *m_parent;
        bool m_dryPaint;
        bool m_showNumber;
        QRegExp m_re;

        QFont m_foundFont;
        QPalette m_foundPalette;
        QFont m_editorFont;
        QPalette m_editorPalette;
        QFont m_numberFont;
        QPalette m_numberPalette;

        // items drawn using QTextDocument
        mutable QList<QWidget*> m_cache;

        /** Return cached item, create it if it doesn't exist. */
        QWidget *cache(const QModelIndex &index) const;

        /** Remove cached item for given @a row. */
        void removeCache(int row) const;

        /** Remove cached item for given @a index. */
        void removeCache(const QModelIndex &index) const;

    signals:
        /** Emitted if item size is changed. */
        void sizeUpdated(const QModelIndex &index) const;

    public slots:
        // change size buffer
        void dataChanged(const QModelIndex &a, const QModelIndex &b);
        void rowsRemoved(const QModelIndex &parent, int start, int end);
        void rowsInserted(const QModelIndex &parent, int start, int end);
        void rowsMoved(const QModelIndex &sourceParent,
                       int sourceStart, int sourceEnd,
                       const QModelIndex &destinationParent,
                       int destinationRow);
};

#endif