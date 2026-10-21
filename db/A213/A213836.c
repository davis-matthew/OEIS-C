/*
 * A213836  Rectangular array: (row n) = b**c, where b(h) = 4*h-3, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where T(n,k) = (n+k)*k*(2*k-1) - k(k+1)(8*k-5)/6
 *            The array is read by falling antidiagonals from (1,1). For a
 *            given n, find t such that t(t+1)/2 >= n, let pos = n - (t-1)*t/2
 *            (1-indexed within the t‑th diagonal), then r = pos, c = t+1 -
 *            pos
 * Range:     u8 n <= 28, u16 n <= 325, u32 n <= 325, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* determine the diagonal number t with t(t+1)/2 >= n */
    idx_t t = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    seq_t cn = (seq_t)n;
    if ((seq_t)t * (t + 1) / 2 < cn) ++t;

    /* previous cumulative sum C(t-1) = (t-1)*t/2 */
    seq_t prev = (seq_t)(t - 1) * t / 2;
    idx_t pos = (idx_t)(cn - prev);          /* 1‑based index on diagonal */
    seq_t r = (seq_t)pos;                     /* row index */
    seq_t c = (seq_t)(t + 1 - pos);           /* column index */

    /* compute T(r,c) */
    seq_t term1 = (r + c) * c * (2 * c - 1);
    seq_t term2 = c * (c + 1) * (8 * c - 5) / 6;
    return term1 - term2;
}
