/*
 * A213835  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          4*n-7+4*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * For n≥1, let d = ⌈(√(8n+1)-1)/2⌉, t_prev = (d–1)d/2. * i = n –
 *            t_prev, j = d – i + 1. * The convolution value is * T(i,j) =
 *            (4*i + 4*j – 3) * j*(j+1)/2 * – 4 * j*(j+1)*(2*j+1)/6. * a(n) =
 *            T(i,j)
 * Range:     u8 n <= 28, u16 n <= 325, u32 n <= 325, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, so n≥1 */

    /* Find diagonal d: smallest integer with d*(d+1)/2 ≥ n */
    long double nd = (long double)n;
    idx_t d = (idx_t)floorl((sqrtl(1.0L + 8.0L*nd) - 1.0L) / 2.0L);
    while ( (d*(d+1))/2 < n ) d++;
    while ( ((d-1)*d)/2 >= n ) d--;

    idx_t t_prev = ((d-1)*d)/2;
    idx_t i = n - t_prev;          /* 1‑based row index */
    idx_t j = d - i + 1;           /* 1‑based column index */

    seq_t big_i = (seq_t)i;
    seq_t big_j = (seq_t)j;

    seq_t S1 = big_j * (big_j + 1) / 2;
    seq_t S2 = big_j * (big_j + 1) * (2 * big_j + 1) / 6;

    seq_t factor = 4 * big_i + 4 * big_j - 3;
    return factor * S1 - 4 * S2;
}
