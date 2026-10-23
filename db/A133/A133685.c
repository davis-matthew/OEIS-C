/*
 * A133685  Let p = prime(n); then a(n) = (sum of prime factors of p+1) - (sum
 *          of prime factors of p-1). a(1) = 2 by convention.
 * https://oeis.org/A133685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A001414(prime(n)+1) - A001414(prime(n)-1), a(1)=2
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

/* Sum the prime factors of m (with multiplicity). */
static seq_t sum_prime_factors_uint64(uint64_t m)
{
    if (m < 2) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    seq_t sum = 0;
    factor_u64(m, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    uint32_t p = get_prime(n);        /* n-th prime */
    uint64_t m1 = (uint64_t)p + 1;    /* prime + 1 */
    uint64_t m2 = (uint64_t)p - 1;    /* prime - 1 */
    seq_t sum1 = sum_prime_factors_uint64(m1);
    seq_t sum2 = sum_prime_factors_uint64(m2);
    return sum1 - sum2;
}
