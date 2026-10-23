/*
 * A191543  Dispersion of (floor(8*n/3)), by antidiagonals.
 * https://oeis.org/A191543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_t.h"
/* A191543: Dispersion of floor(8*n/3) by antidiagonals.
   The array D is defined by D[row i , col j] =
   (t[i], s(t[i]), s(s(t[i])), ...), where
   s(k) = floor(8*k/3) and t is the complement of s.
   The nth term a(n) is the nth element of D read by
   antidiagonals starting from the northwest corner. */

seq_t a(idx_t n)
{
    /* locate the coordinates (i,j) of the nth antidiagonal element */
    idx_t k = 1;
    idx_t cum = 1; /* cumulative count of elements up to diagonal k */
    while (cum < n) {
        ++k;
        cum += k;
    }
    idx_t prev_cum = cum - k; /* elements before diagonal k */
    idx_t p = n - prev_cum;   /* 1‑based index within diagonal k */
    idx_t i = p;              /* row index */
    idx_t j = k - p + 1;      /* column index */

    /* first element in row i is t(i) */
    seq_t val = get_t(i);

    /* apply s j-1 times to get the jth element in that row */
    for (idx_t r = 1; r < j; ++r)
        val = ((seq_t)8 * val) / 3;

    return val;
}
