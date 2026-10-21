/*
 * A200768  Sum of the n-th powers of the distinct prime divisors of n.
 * https://oeis.org/A200768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} p^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"

/* Cache of all primes up to the largest n for which a() has been called. */
static uint64_t *prime_cache = NULL;
static size_t   prime_count = 0;
static idx_t    cache_limit  = 0;

/* Ensure that prime_cache contains all primes ≤ n. */
static void ensure_primes(idx_t n)
{
    if (n <= cache_limit)
        return;
    /* Rebuild the prime list if a larger limit is required. */
    if (prime_cache)
        free(prime_cache);
    build_primes_upto(n, &prime_cache, &prime_count);
    cache_limit = n;
}

seq_t a(idx_t n)
{
    /* n = 1 has no prime divisors, so a(1) = 0. */
    if (n <= 1)
        return (seq_t)0;

    ensure_primes(n);

    seq_t sum = (seq_t)0;
    idx_t temp = n;

    /* Factor n and accumulate the sum of distinct prime powers. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_cache[i];
        /* stop once p^2 > temp (no larger prime divisor). */
        if (p > temp / p)
            break;
        if (temp % p == 0) {
            sum += pow_uint(p, n);
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* Any remaining factor is a prime > sqrt(original n). */
    if (temp > 1)
        sum += pow_uint(temp, n);

    return sum;
}
