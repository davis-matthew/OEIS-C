/*
 * A083744  a(1) = 1; if a(n-1) + n is composite then a(n) = a(n-1) + n, else
 *          a(n) = a(n-1).
 * https://oeis.org/A083744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1;
   a(n) = a(n‑1) + n   if a(n‑1) + n is composite,
   a(n) = a(n‑1)       otherwise.      */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrime.h"   /* declares bool isPrime(idx_t) */

seq_t a(idx_t n)
{
    /* 1‑based cache of already computed terms */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static idx_t  cache_cap    = 0; /* allocated capacity minus one (max index that can be accessed) */
    static idx_t  cache_size   = 0; /* highest n that has been computed */

    if (n < 1) return (seq_t)0;   /* undefined for n = 0 */

    /* Initialise cache on first entry */
    if (cache == NULL) {
        cache_cap = 2;                          /* at least two slots (indices 0…2) */
        cache     = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) { exit(EXIT_FAILURE); }
        cache[1] = (seq_t)1;      /* a(1) = 1 */
        cache_size = 1;
    }

    /* Grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n + 1;   /* we need indices 0…new_cap */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) { exit(EXIT_FAILURE); }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t cand   = prev + (seq_t)i;          /* candidate value */

        /* Test if candidate is composite (i.e., not prime and ≠ 1) */
        if (cand > (seq_t)1 && !isPrime((idx_t)cand))
            cache[i] = cand;    /* use candidate if composite */
        else
            cache[i] = prev;    /* keep previous value if candidate is prime */
    }

    cache_size = n;
    return cache[n];
}
