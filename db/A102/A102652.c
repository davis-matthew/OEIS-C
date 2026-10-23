/*
 * A102652  a(n) = 4 * floor(24*2^n/15) = 4*A077854(n).
 * https://oeis.org/A102652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 4 * floor(24*2^n/15) = 4*A077854(n)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 4;                /* a(0) */
    if (n == 0) return res;

    /* m holds (3*2^k) mod 5 for the current k.
       Initially k = 0, so m = 3. */
    uint64_t m = 3;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* a(k+1) = 2*a(k) + 4*reg where reg is 0 or 1. */
        res <<= 1;                 /* multiply by 2 */
        if (m > 2)                 /* reg = 1 iff (3*2^k) mod 5 > 2 */
            res += 4;              /* add 4 if reg = 1 */

        /* Update m for next power of two */
        m = (m << 1) % 5;          /* (3*2^{k+1}) mod 5 */
    }

    return res;
}
