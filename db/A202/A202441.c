/*
 * A202441  Number of (n+2) X 4 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5) for
 *            n>6 with initial values a(1)=138, a(2)=226, a(3)=376, a(4)=574,
 *            a(5)=836, a(6)=1162
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoisation storage */
static idx_t cache_size = 0;       /* current allocated size of cache (indexable up to cache_size-1) */
static idx_t max_calc   = 0;       /* highest index for which a(i) has been computed */

/* Return a(n) for the sequence A202441.  n is assumed to be >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below offset is unimportant */

    /* ensure cache is large enough for the requested n */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = (cache_size > n ? cache_size : n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
        /* initialise base cases if they have not yet been set */
        if (max_calc < 6) {
            if (cache_size > 1) cache[1] = 138;
            if (cache_size > 2) cache[2] = 226;
            if (cache_size > 3) cache[3] = 376;
            if (cache_size > 4) cache[4] = 574;
            if (cache_size > 5) cache[5] = 836;
            if (cache_size > 6) cache[6] = 1162;
            max_calc = (cache_size > 6 ? 6 : cache_size-1);
        }
    }

    /* Compute values only for indices > 6 using the recurrence */
    idx_t start = (max_calc < 7) ? 7 : max_calc + 1;
    if (n >= start) {
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 3*cache[i-1] - 2*cache[i-2] - 2*cache[i-3]
                       + 3*cache[i-4] - cache[i-5];
        }
        max_calc = n;
    }

    return cache[n];
}
