/*
 * A048915  9-gonal pentagonal numbers.
 * https://oeis.org/A048915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146361602*a(n-2) - a(n-4) + 35719200 * with initial
 *            values a(1)=1, a(2)=651, a(3)=180868051, a(4)=95317119801
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Handle indices below the offset safely */
    if (n < 1) return (seq_t)0;

    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_used = 0;

    /* Ensure cache is at least large enough for index n */
    if (n >= cache_cap) {
        /* Grow capacity: at least n+1, double each time for efficiency */
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure, return 0 as fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first four values lazily */
    if (cache_used < 5) {
        cache[1] = (seq_t)1ULL;
        cache[2] = (seq_t)651ULL;
        cache[3] = (seq_t)180868051ULL;
        cache[4] = (seq_t)95317119801ULL;
        cache_used = 5;  /* indices 1..4 are set */
    }

    /* Compute any missing values up to n using the recurrence */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)146361602ULL * cache[i - 2]
                     - cache[i - 4]
                     + (seq_t)35719200ULL;
        cache[i] = term;
    }
    cache_used = (size_t)n + 1;  /* we've now computed up to n */

    return cache[n];
}
