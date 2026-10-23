/*
 * A139292  2^(2p - 1)/8, where p is prime.
 * https://oeis.org/A139292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2p(n)-1)/8 where p(n) is the n-th prime
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
    /* The sequence is defined for n >= 1. For n < 1 it simply returns 0. */
    if (n < 1)
        return 0;

    /* Fetch the n-th prime (p(1)=2, p(2)=3, …). */
    uint32_t p = get_prime(n);

    /* a(n) = 2^(2p-1)/8 = 2^(2p-4).  Compute the exponent. */
    idx_t exp = (idx_t)2 * (idx_t)p - 4;

    /* Return 2^exponent as a seq_t value. */
    return pow2(exp);
}
