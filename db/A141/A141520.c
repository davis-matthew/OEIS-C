/*
 * A141520  a(1) = 0, a(2) = a(3) = 1; a(n) = a(n-1)*a(n-2) + a(n-2)*a(n-3)
 *          for n > 3.
 * https://oeis.org/A141520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=a(3)=1; a(n)=a(n-1)*a(n-2)+a(n-2)*a(n-3) for n>3
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated capacity (indexes 0..cache_cap-1) */
static idx_t max_computed = 0;    /* highest n for which cache[n] is valid */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety; OEIS offset is 1 */

    /* Ensure the cache can hold all indices up to n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure → zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms if they are not yet stored */
    while (max_computed < 3u && max_computed < n) {
        switch (max_computed) {
            case 0u: cache[1] = (seq_t)0; break; /* a(1) */
            case 1u: cache[2] = (seq_t)1; break; /* a(2) */
            case 2u: cache[3] = (seq_t)1; break; /* a(3) */
        }
        ++max_computed;
    }

    /* If the requested term is already known, return it */
    if (n <= max_computed) return cache[n];

    /* Compute remaining terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        cache[i] = t1 * t2 + t2 * t3;
    }
    max_computed = n;
    return cache[n];
}
