/*
 * A079102  a(2n) = 2^n, a(2n+1) = 2^(2n).
 * https://oeis.org/A079102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 2^n, a(2n+1) = 2^(2n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = n >> 1;                  /* floor(n/2)   */
    idx_t exp = (n & 1) ? (m << 1) : m;/* exponent for pow2 */
    return pow2(exp);
}
