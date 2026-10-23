/*
 * A008288  Square array of Delannoy numbers D(i,j) (i >= 0, j >= 0) read by
 *          antidiagonals.
 * https://oeis.org/A008288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) is obtained by reading the square array of Delannoy numbers
 *            D(i,j) (i >= 0, j >= 0) by antidiagonals. For an index n
 *            (0‑based) let s be the unique integer with (s+1)(s+2)/2 > n and
 *            w = n - s*(s+1)/2. Then a(n) = D(w, s-w), where D(i,j) =
 *            Σ_{d=0..min(i,j)} binomial(i,d) * binomial(j,d) * 2^d
 * Range:     u8 n <= 39, u16 n <= 125, u32 n <= 417, u64 n <= 1455,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

static seq_t delannoy(idx_t n, idx_t k)
{
    idx_t m = (n < k) ? n : k;
    seq_t sum = 0;
    for (idx_t d = 0; d <= m; ++d)
    {
        seq_t term = binomial(n, d);
        term *= binomial(k, d);
        term *= (seq_t)1 << d;  /* 2^d */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find s such that (s+1)(s+2)/2 > n */
    idx_t s = (idx_t)((sqrt((long double)8 * n + 1) - 1) / 2);
    /* adjust in case of floating point rounding */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t w = n - s * (s + 1) / 2;   /* offset within the antidiagonal */
    idx_t i = w;                     /* row index */
    idx_t j = s - w;                 /* column index */

    return delannoy(i, j);
}
