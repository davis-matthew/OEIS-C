/*
 * A123566  Table (read by antidiagonals) where t(1,n) = t(m,1) = 1 and where
 *          t(m,n) = lcm(Sum_{k=1..m-1} t(k,n), Sum_{j=1..n-1} t(m,j)).
 * https://oeis.org/A123566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 39, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   t(1,n)  = 1
 *   t(m,1)  = 1
 *   t(m,n)  = lcm( Σ_{k=1}^{m-1} t(k,n) , Σ_{j=1}^{n-1} t(m,j) )
 *   a(n)    = value of the table read by antidiagonals (offset 1)
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Map n to table coordinates (m, col) */
    size_t diag = 1;
    while ((diag * (diag + 1)) / 2 < (size_t)n)
        ++diag;
    size_t prev  = (diag - 1) * diag / 2;
    size_t offset = (size_t)n - prev - 1;          /* 0‑based index on the diagonal */
    size_t m = offset + 1;                         /* 1‑based row */
    size_t col = diag - offset;                    /* 1‑based column */

    /* Determine size of table needed (square matrix). */
    size_t needed = (m > col) ? m : col;

    /* Static cache of the table. */
    static seq_t *table = NULL;
    static size_t curSize = 0;
    size_t oldSize = curSize;   /* size of table before possible resize */

    if (needed > curSize) {
        size_t newSize = needed;
        seq_t *newTable = (seq_t *)malloc(newSize * newSize * sizeof(seq_t));
        if (!newTable)
            return (seq_t)0;   /* out‑of‑memory – unlikely */
        /* copy existing cells into new layout */
        for (size_t i = 0; i < curSize; ++i)
            for (size_t j = 0; j < curSize; ++j)
                newTable[i * newSize + j] = table[i * curSize + j];
        free(table);
        table   = newTable;
        curSize = newSize;
    }

    /* Compute any new cells that were not already present. */
    for (size_t i = 1; i <= curSize; ++i) {
        for (size_t j = 1; j <= curSize; ++j) {
            size_t idx = (i - 1) * curSize + (j - 1);
            if (i == 1 || j == 1) {
                table[idx] = (seq_t)1;
                continue;
            }
            if (i > oldSize || j > oldSize) {
                seq_t sumCol = 0;
                for (size_t k = 1; k < i; ++k)
                    sumCol += table[(k - 1) * curSize + (j - 1)];
                seq_t sumRow = 0;
                for (size_t k = 1; k < j; ++k)
                    sumRow += table[(i - 1) * curSize + (k - 1)];
                seq_t g = gcd(sumCol, sumRow);
                table[idx] = (g == 0) ? (seq_t)0 : (sumCol / g) * sumRow;
            }
        }
    }

    /* Return requested entry. */
    return table[(m - 1) * curSize + (col - 1)];
}
