/*
 * A174855  y-values in the solution to x^2-51*y^2=1.
 * https://oeis.org/A174855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) - a(n-2) with a(1)=0, a(2)=7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return a(n) for the OEIS sequence A174855. */
seq_t a(idx_t n)
{
    /* Cache for memoisation. */
    static seq_t *cache   = NULL;   /* array of computed terms (index 1..). */
    static idx_t  cache_sz = 0;     /* highest index currently stored. */
    static size_t cap      = 0;     /* allocated capacity (indices 0..cap-1). */

    /* Ensure the cache can hold indices up to n. */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (cap == 0 ? 3 : cap);
        /* grow until we have room for n (and at least indices 0..2). */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* OOM fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first two terms when the cache is empty. */
    if (cache_sz < 2) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)7;
        cache_sz = 2;
    }

    /* Compute any missing terms up to n. */
    if (n > cache_sz) {
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            cache[i] = (seq_t)(100 * cache[i - 1] - cache[i - 2]);
        }
        cache_sz = n;
    }

    return cache[n];
}
