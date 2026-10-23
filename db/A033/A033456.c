/*
 * A033456  LCM-convolution of squares A000290 with themselves.
 * https://oeis.org/A033456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} lcm((k+1)^2, (n-k+1)^2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute (k+1)^2 and (n-k+1)^2 */
        seq_t kk = (seq_t)(k + 1);
        seq_t x  = kk * kk;
        seq_t nk = (seq_t)(n - k + 1);
        seq_t y  = nk * nk;

        /* lcm(x,y) = x/gcd(x,y) * y */
        seq_t g = gcd(x, y);
        seq_t l = (x / g) * y;
        sum += l;
    }
    return sum;
}
