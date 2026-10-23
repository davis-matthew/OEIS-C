/*
 * A156875  Number of safe primes <= n.
 * https://oeis.org/A156875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of safe primes <= n a safe prime is a prime p
 *            such that (p-1)/2 is also prime (i.e. p = 2q+1 where q is prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   // provides bool is_prime(idx_t)

/* compute a(n) */
seq_t a(idx_t n)
{
    /* static cache of computed values: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;      /* highest index cached */

    if (n == 0)                          /* for completeness */
        return 0;

    /* allocate or grow cache array if needed */
    if (cache == NULL) {
        cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        /* calloc zeroes the array, so cache[i] = 0 for all i */
        cache_max = 0;
        cache[0] = 0;    /* as defined by the sequence */
    } else if (n > cache_max) {
        /* extend the cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {      /* allocation failed */
            return 0;
        }
        /* initialize the newly allocated part to 0 */
        for (idx_t i = cache_max + 1; i <= n; ++i)
            new_cache[i] = 0;
        cache = new_cache;
    }

    /* if we already have the value, just return it */
    if (n <= cache_max)
        return cache[n];

    /* compute values incrementally from cache_max+1 up to n */
    seq_t count = cache[cache_max];
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* start with previous count */
        count = cache[i - 1];
        /* check if i is a safe prime */
        if (is_prime(i) && is_prime((i - 1) / 2)) {
            ++count;
        }
        cache[i] = count;
    }

    /* update cache_max */
    cache_max = n;
    return cache[n];
}
