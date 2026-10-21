/*
 * A236459  Regular triangle: T(n, k) Manhattan distance between n and k in a
 *          left-aligned triangle with next M natural numbers in row M.
 * https://oeis.org/A236459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Manhattan distance between the numbers i = n − (j −
 *            1)·j/2 and j = row index of n, where the row index j is the
 *            minimal integer such that j·(j + 1)/2 ≥ n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* row index of n */
    idx_t j = row_of(n);

    /* column index of n (= number i) */
    idx_t i = n - (j - 1) * j / 2;

    /* coordinates of number i */
    idx_t row_i = row_of(i);
    idx_t col_i = i - (row_i - 1) * row_i / 2;

    /* coordinates of number j (the row number itself as a triangle element) */
    idx_t row_j = row_of(j);
    idx_t col_j = j - (row_j - 1) * row_j / 2;

    /* Manhattan distance (cast to seq_t) */
    seq_t dr = (seq_t)(row_i > row_j ? row_i - row_j : row_j - row_i);
    seq_t dc = (seq_t)(col_i > col_j ? col_i - col_j : col_j - col_i);
    return dr + dc;
}
