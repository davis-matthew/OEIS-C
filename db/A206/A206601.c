/*
 * A206601  a(n) = 3^(n(n+1)/2) - 1.
 * https://oeis.org/A206601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n*(n+1)/2) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = n*(n+1)/2 */
    idx_t k = n * (n + 1) / 2;
    /* compute 3^k */
    seq_t res = big_pow((seq_t)3, (seq_t)k);
    /* subtract 1 to get a(n) */
    return res - 1;
}
