/*
 * A076131  a(n) = 2^n*a(n-1) + 1, a(0) = 0.
 * https://oeis.org/A076131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 2^n * a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val  = (seq_t)0;   /* a(0) */
    seq_t pow2 = (seq_t)1;   /* 2^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;                /* pow2 = 2^i */
        val = pow2 * val + (seq_t)1;
    }
    return val;
}
