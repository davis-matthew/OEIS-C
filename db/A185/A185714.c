/*
 * A185714  a(n) = number of primes <= n that end in 7.
 * https://oeis.org/A185714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of primes <= n that end in digit 7
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Static state keeps the sieve and prefix sums across calls. */
    static bool *is_prime = NULL;     /* prime flags 0 .. max_n */
    static idx_t sieve_max = 0;        /* size of is_prime (max index + 1) */
    static seq_t *count_cache = NULL; /* counts a(i) for i < cache_max */
    static idx_t cache_max = 0;        /* size of count_cache (max index + 1) */

    /* If n is beyond the current sieve, extend it. */
    if (n >= sieve_max)
    {
        idx_t new_max = n + 1;         /* allocate indices 0 .. n */

        /* Reallocate the prime flag array. */
        is_prime = (bool *)realloc(is_prime, new_max * sizeof(bool));
        for (idx_t i = sieve_max; i < new_max; ++i)
            is_prime[i] = true;        /* new entries are initially true */

        /* 0 and 1 are not prime. */
        if (new_max > 0) is_prime[0] = false;
        if (new_max > 1) is_prime[1] = false;

        /* Sieve of Eratosthenes up to n. */
        for (idx_t p = 2; p * p <= (idx_t)n; ++p)
        {
            if (is_prime[p])
            {
                for (idx_t j = p * p; j <= (idx_t)n; j += p)
                    is_prime[j] = false;
            }
        }

        /* Reallocate the cache for the counts. */
        count_cache = (seq_t *)realloc(count_cache, new_max * sizeof(seq_t));

        /* Initialize the cache if this is the first expansion. */
        if (cache_max == 0)
        {
            count_cache[0] = (seq_t)0;
            cache_max = 1;
        }

        /* Fill the cache from cache_max up to n. */
        for (idx_t i = cache_max; i <= (idx_t)n; ++i)
        {
            seq_t add = (is_prime[i] && ((i % 10) == 7)) ? (seq_t)1 : (seq_t)0;
            count_cache[i] = count_cache[i - 1] + add;
        }

        sieve_max = new_max;
        cache_max = new_max;
    }

    return count_cache[n];
}
