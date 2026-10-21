/*
 * A228035  Triangle T(n,k), read by rows: T(n,k) is the denominator of
 *          (1-2^(n-k+1))/(1-2^(k+1)).
 * https://oeis.org/A228035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = (2^(k+1)-1)/(2^gcd(n+2,k+1)-1), where k = n - n(n+1)/2 is
 *            the column index obtained by flattening the triangle with rows
 *            of length n+1
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the row (r) and column (c) of the element with index
   idx in the flattened triangle.  Rows are numbered starting at 0. */
static inline void row_col_from_index(idx_t idx, idx_t *r, idx_t *c) {
    idx_t row = 0;
    idx_t tri = 0; /* total number of elements before the current row */
    while (tri + row + 1 <= idx) {
        row++;
        tri += row;
    }
    *r = row;
    *c = idx - tri;
}

seq_t a(idx_t idx) {
    idx_t row, col;
    row_col_from_index(idx, &row, &col);

    idx_t m = col + 1;                     /* k+1 */
    idx_t g = gcd_idx(row + 2, m);          /* gcd(n+2,k+1) */

    seq_t top    = pow2(m) - (seq_t)1;      /* 2^m - 1 */
    seq_t bottom = pow2(g) - (seq_t)1;      /* 2^g - 1 */

    return top / bottom;                    /* denominator of reduced fraction */
}
