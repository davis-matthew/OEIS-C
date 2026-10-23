/*
 * A187469  Array: five joint rank sequences tending to lower Wythoff sequence
 *          A000201, by columns.
 * https://oeis.org/A187469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th column‑wise entry of the array of five Adjusted
 *            Joint Rank Sequences (AJRS) of the natural numbers N. Row 1 is
 *            the odd numbers 1,3,5,…. For r ≥ 2, the r‑th row is defined by
 *            a_r(k) = k + #{ t : a_{r-1}(t) < k } for k ≥ 1. The sequence
 *            a(n) is read column‑wise (offset 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined below offset */

    /* Number of columns needed for the requested n */
    size_t colCount = (size_t)((n - 1) / 5) + 1;

    /* Allocate five arrays, each indexed from 1 to colCount */
    seq_t *row[5];
    for (int i = 0; i < 5; ++i) {
        row[i] = seq_alloc_array(colCount + 1);   /* index 0 unused */
        if (!row[i]) {
            for (int j = 0; j < i; ++j) seq_free_array(row[j]);
            return 0;
        }
    }

    /* Row 1: odd numbers 1,3,5,… */
    for (size_t k = 1; k <= colCount; ++k) {
        row[0][k] = (seq_t)((seq_t)2 * k - 1);
    }

    /* Generate rows 2 through 5 */
    for (int r = 1; r < 5; ++r) {
        size_t p = 1;          /* next position in previous row to test */
        seq_t cnt = 0;         /* count of previous‑row values < current k */

        for (size_t k = 1; k <= colCount; ++k) {
            while (p <= colCount && row[r-1][p] < (seq_t)k) {
                ++p;
                ++cnt;
            }
            row[r][k] = (seq_t)k + cnt;
        }
    }

    /* Determine the target row and column */
    size_t column = (size_t)((n - 1) / 5) + 1;   /* 1‑based column index */
    int   rowIdx  = (int)((n - 1) % 5);          /* 0‑based row index (0..4) */

    seq_t result = row[rowIdx][column];

    /* Free memory */
    for (int i = 0; i < 5; ++i) seq_free_array(row[i]);

    return result;
}
