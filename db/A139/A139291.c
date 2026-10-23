/*
 * A139291  a(n) = 2^(2*prime(n) - 3) - 1.
 * https://oeis.org/A139291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*prime(n) - 3) - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 */
    if (n < 1) return 0;
    uint32_t p = get_prime(n);
    idx_t exp = (idx_t)2 * p - 3;
    seq_t val = pow_uint(2, exp);
    val -= 1;
    return val;
}
