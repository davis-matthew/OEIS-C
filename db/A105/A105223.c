/*
 * A105223  Number of squares between prime(n) and 2*prime(n) inclusive.
 * https://oeis.org/A105223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000196(2*prime(n)) - A000196(prime(n)-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_prime_upper_bound, isqrt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count squares k^2 with prime(n) ≤ k^2 ≤ 2*prime(n). */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/isqrt64.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset 1: caller will never ask for n = 0 */
        return 0;

    /* static storage for primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    if (primes_cnt < (size_t)n)
    {
        /* Ensure enough space for new primes */
        uint64_t needed = (uint64_t)n * 2;
        if (primes_cap < needed)
        {
            uint64_t *new_arr = (uint64_t *)realloc(primes, needed * sizeof(uint64_t));
            if (!new_arr)
                return 0;                     /* allocation failure */
            primes = new_arr;
            primes_cap = needed;
        }

        /* Compute a new sieve up to an upper bound for the nth prime */
        uint64_t limit = estimate_prime_upper_bound((idx_t)n);
        if (limit < 20)
            limit = 20;                     /* guard for very small n */

        char *is_prime = (char *)malloc(limit + 1);
        if (!is_prime)
            return 0;                      /* allocation failure */
        for (uint64_t i = 0; i <= limit; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        for (uint64_t i = 2; i * i <= limit; ++i)
        {
            if (is_prime[i])
            {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Collect primes until we have enough */
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit && cnt < (size_t)n; ++i)
            if (is_prime[i])
                primes[cnt++] = i;

        primes_cnt = cnt;
        free(is_prime);
    }

    /* Now primes[n-1] holds the nth prime */
    uint64_t p = primes[n - 1];
    uint64_t two_p = 2 * p;

    uint64_t ceil_sqrt_p = isqrt64(p);
    if (ceil_sqrt_p * ceil_sqrt_p < p)
        ++ceil_sqrt_p;

    uint64_t floor_sqrt_2p = isqrt64(two_p);

    seq_t result = (seq_t)(floor_sqrt_2p - ceil_sqrt_p + 1);
    return result;
}
