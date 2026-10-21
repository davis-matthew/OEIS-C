/*
 * A062160  Square array T(n,k) = (n^k - (-1)^k)/(n+1), n >= 0, k >= 0, read
 *          by falling antidiagonals.
 * https://oeis.org/A062160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = (r^c - (-1)^c)/(r+1). The indices
 *            are obtained by reading the square array T(n,k) along falling
 *            antidiagonals: for each diagonal d (sum index), enumerate
 *            elements with row r=0..d and column c = d-r. Thus for a given
 *            index n, let d satisfy d(d+1)/2 ≤ n < (d+1)(d+2)/2,...
 * Range:     i8 n <= 47, i16 n <= 94, i32 n <= 214, i64 n <= 473,
 *            i128 n <= 1235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* Correct possible rounding error */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    idx_t m = n - d * (d + 1) / 2;
    idx_t r = m;          /* row index */
    idx_t c = d - m;      /* column index */

    /* Compute T(r,c) using recurrence */
    if (c == 0) return 0;
    seq_t val = 0;
    for (idx_t k = 1; k <= c; ++k) {
        /* same as: val = val * r - ((k % 2 == 0) ? 1 : -1); */
        val = val * r;
        if (k & 1) /* k is odd */
            val += 1;
        else
            val -= 1;
    }
    return val;
}
