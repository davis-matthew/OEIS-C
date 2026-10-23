/*
 * A135174  a(n) = 5^prime(n) - 3^prime(n) + 2^prime(n).
 * https://oeis.org/A135174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^prime(n) - 3^prime(n) + 2^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    seq_t pow5 = pow_uint(5, (idx_t)p);
    seq_t pow3 = pow_uint(3, (idx_t)p);
    seq_t pow2 = pow_uint(2, (idx_t)p);
    return pow5 - pow3 + pow2;
}
