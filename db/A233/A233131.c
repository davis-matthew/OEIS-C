/*
 * A233131  Sum of remainders of n modulo all smaller composite numbers.
 * https://oeis.org/A233131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{c composite, c<n} n mod c
 * Range:     u8 n <= 45, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Static cache for primes up to the largest limit seen. */
static bool *is_prime_cache = NULL;
static size_t cache_max = 0;

/* Compute a(n) as the sum of n modulo all composite numbers smaller than n. */
seq_t a(idx_t n)
{
    /* No composite numbers below 4. */
    if (n <= 4)
        return 0;

    /* Ensure we have a prime sieve up to n-1. */
    size_t limit = (size_t)(n - 1);
    if (limit > cache_max) {
        /* Allocate (or reallocate) the sieve array. */
        bool *tmp = (bool*)realloc(is_prime_cache, (limit + 1) * sizeof(bool));
        if (!tmp)
            return 0; /* allocation failure – return zero to avoid UB */
        is_prime_cache = tmp;

        /* Sieve of Eratosthenes up to 'limit'. */
        for (size_t i = 0; i <= limit; ++i)
            is_prime_cache[i] = true;
        if (limit >= 0) is_prime_cache[0] = false;
        if (limit >= 1) is_prime_cache[1] = false;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (is_prime_cache[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime_cache[j] = false;
            }
        }
        cache_max = limit;
    }

    /* Compute the sum of remainders. */
    seq_t sum = 0;
    seq_t sn = (seq_t)n;
    for (size_t i = 4; i < (size_t)n; ++i) {
        if (!is_prime_cache[i])            /* i is composite */
            sum += sn % (seq_t)i;
    }

    return sum;
}
