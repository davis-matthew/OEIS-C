/*
 * A059917  a(n) = (3^(2^n) + 1)/2 = A059919(n)/2, n >= 0.
 * https://oeis.org/A059917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (3^(2^n) + 1)/2 (OEIS A059917) Recurrence: a(0) = 2; a(n)
 *            = ((2*a(n-1) - 1)^2 + 1) / 2 for n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t if needed by the build */

seq_t a(idx_t n)
{
    seq_t val = 2;          /* a(0) */
    if (n == 0)
        return val;

    /* iterate using the recurrence */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t t = (val << 1) - 1;   /* 2*val - 1 = 3^(2^(i-1)) */
        t = t * t + 1;              /* (3^(2^(i-1)))^2 + 1 = 3^(2^i) + 1 */
        val = t >> 1;               /* divide by 2 */
    }
    return val;
}
