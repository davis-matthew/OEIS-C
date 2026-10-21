/*
 * A061236  Smallest number with prime(n)^3 divisors where prime(n) is n-th
 *          prime.
 * https://oeis.org/A061236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 for n=1; a(n) = 30^(prime(n)-1) for n>1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=24, a(n)=30^(p_n-1) where p_n is the n-th prime */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)24;

    uint32_t p = get_prime(n);          /* p is the n-th prime (>=3 for n>1) */
    idx_t exp = (idx_t)(p - 1);           /* exponent for 30 */

    return seq_pow((seq_t)30, exp);
}
