/*
 * A141083  a(n) = 2^(p - 2)*(2^p - 2), where p = prime(n).
 * https://oeis.org/A141083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p-2)*(2^p-2), where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 */
    uint32_t p = get_prime(n);

    seq_t two_pow_pminus2 = pow2((idx_t)(p - 2));
    seq_t two_pow_p = pow2((idx_t)p);

    return two_pow_pminus2 * (two_pow_p - (seq_t)2);
}
