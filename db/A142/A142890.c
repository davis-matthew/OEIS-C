/*
 * A142890  Primes congruent to 2 mod 63.
 * https://oeis.org/A142890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 2 mod 63 (OEIS A142890)
 * Range:     u8 n <= 2, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, guard clause */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    /* if we already have the requested term, return it */
    if (cache && (size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* determine upper bound for sieve using a simple overestimation   */
    /*  n * 600 is safe for n up to about 10 000; we also ensure a minimum of 6 000 000 */
    size_t limit = (size_t)n * 600ULL;
    if (limit < 6000000ULL)
        limit = 6000000ULL;

    /* simple Eratosthenes sieve up to 'limit' */
    char *marked = (char *)malloc(limit + 1);
    if (!marked) return 0;            /* allocation failed */
    for (size_t i = 0; i <= limit; ++i)
        marked[i] = 0;

    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sqrt_limit; ++i) {
        if (!marked[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                marked[j] = 1;
        }
    }

    /* count desired primes to allocate cache */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!marked[i] && (i % 63ULL) == 2ULL)
            ++count;
    }

    /* build cache */
    seq_t *new_cache = (seq_t *)malloc(count * sizeof(seq_t));
    if (!new_cache) {
        free(marked);
        return 0;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!marked[i] && (i % 63ULL) == 2ULL)
            new_cache[idx++] = (seq_t)i;
    }
    free(marked);
    free(cache);      /* free previous cache if any */
    cache = new_cache;
    cache_len = count;

    /* return the requested term (guaranteed to exist after the above) */
    return cache[(size_t)n - 1];
}
