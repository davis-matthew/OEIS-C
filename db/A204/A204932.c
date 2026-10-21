/*
 * A204932  Least k such that n divides k!-j! for some j satisfying 1<=j<k.
 * https://oeis.org/A204932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5692, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the smallest k such that n | k! – j! for some 1 ≤ j < k.
   It is the first repeated residue in the sequence k! mod n, starting from
   1! mod n = 1.  Computation and memoization are both O(n). */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* seq_t and idx_t are defined by the build (e.g. uint8_t, uint64_t, …). */

seq_t a(idx_t n)
{
    /* Cache for previously computed values: cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;      /* number of elements currently allocated */

    /* Undefined for n = 0 – return 0 as a safe value. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the cache is large enough. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                /* indices 0..n */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            abort();                           /* allocation failure */
        /* Copy old values if any. */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        /* Zero‑initalize new entries. */
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }

    /* Return memoized result if available. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Allocate array to record the first occurrence of each residue. */
    idx_t *first = (idx_t *)calloc(n, sizeof(idx_t));   /* positions 1..k */
    if (!first)
        abort();

    /* Start with 1! mod n. */
    idx_t rem = 1 % n;
    first[rem] = 1;            /* record position 1 */

    for (idx_t k = 2; ; ++k) {
        /* Compute (rem * k) % n without overflow (rem and k ≤ n). */
        rem = (idx_t)(((__int128)rem * k) % n);
        if (first[rem]) {      /* residue repeats – we found the answer */
            cache[n] = (seq_t)k;
            free(first);
            return cache[n];
        }
        first[rem] = k;        /* remember the first occurrence */
    }
}
