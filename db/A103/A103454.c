/*
 * A103454  a(n) = 0^n + 4^n - 1.
 * https://oeis.org/A103454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 4^n - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n = 0 → 0^0 + 4^0 - 1 = 1 */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    /* For n > 0, 0^n = 0, so a(n) = 4^n - 1 = 2^(2n) - 1 */
    seq_t val = pow2(n * (idx_t)2);  /* 2^(2n) */
    return val - (seq_t)1;
}
