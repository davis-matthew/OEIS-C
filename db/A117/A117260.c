/*
 * A117260  Triangle T, read by rows, where matrix inverse T^-1 has -2^n in
 *          the secondary diagonal: [T^-1](n+1,n) = -2^n, with all 1's in the
 *          main diagonal and zeros elsewhere.
 * https://oeis.org/A117260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^( (m-1)*m/2 - d*(d-1)/2 ), where * m = ⌊ (√(8n+1) -
 *            1)/2 ⌋ is the largest integer with m(m+1)/2 ≤ n, * d = n -
 *            m(m+1)/2
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Find m such that t_m = m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    while (m * (m + 1) / 2 <= n) ++m;
    --m;                           /* now m*(m+1)/2 <= n */

    idx_t t_m   = m * (m + 1) / 2;
    idx_t d     = n - t_m;          /* offset inside the block */
    idx_t t_prev = (m > 0) ? (m - 1) * m / 2 : 0;
    idx_t sub    = d * (d - 1) / 2; /* binom(d,2) */

    idx_t exp = t_prev - sub;       /* exponent of 2 */

    return pow2(exp);
}
