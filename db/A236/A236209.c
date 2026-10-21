/*
 * A236209  Sum of the seventh powers of the first n primes.
 * https://oeis.org/A236209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 80, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Prime retrieval helper provided by the build system */
#include "../utils/get_prime.h"

// The function to be implemented
seq_t a(idx_t n)
{
    /* Cached cumulative sums: cache[i-1] == a(i) */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;  /* how many entries are populated */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    if (n < 1)            /* offset is 1; undefined for n < 1 */
        return (seq_t)0;

    /* If value already computed, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure capacity for up to n elements */
    if (n > cache_cap) {
        if (cache_cap == 0) cache_cap = 1;
        while (n > cache_cap)
            cache_cap <<= 1;                     /* double */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Start from the last known sum (or zero if none) */
    seq_t sum = (cache_len > 0) ? cache[cache_len - 1] : 0;

    /* Compute sums for missing entries */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        uint32_t p  = get_prime(k);   /* prime(k) */
        seq_t    p7 = p;              /* p^1 */
        for (int j = 1; j < 7; ++j)   /* compute p^7 */
            p7 *= p;
        sum += p7;
        cache[k - 1] = sum;
    }

    cache_len = n;
    return cache[n - 1];
}
