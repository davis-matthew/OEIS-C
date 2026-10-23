/*
 * A121512  a(n) = a(n-1) + a(n-3) - a(n-4) for n>4, with a(1)=1, a(2)=4,
 *          a(3)=10, a(4)=4.
 * https://oeis.org/A121512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) Initial values: a(1)=1, a(2)=4,
 *            a(3)=10, a(4)=4 Computes terms for all n >= 1 using memoised
 *            recurrence
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms (1‑based indexing) */
static size_t cache_cap = 0;     /* current allocated size */
static idx_t cache_len = 0;      /* number of terms stored (max index computed) */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safeguard */

    /* allocate / grow the cache if needed */
    if (cache == NULL) {
        /* start with a reasonable capacity */
        cache_cap = (size_t)((n > 16) ? n : 16);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if the term is already computed, return it */
    if (n <= cache_len) return cache[n-1];

    /* initialise base cases if they have not yet been set */
    if (cache_len < 4) {
        if (cache_len < 1 && n >= 1) { cache[0] = 1; cache_len = 1; }
        if (cache_len < 2 && n >= 2) { cache[1] = 4; cache_len = 2; }
        if (cache_len < 3 && n >= 3) { cache[2] = 10; cache_len = 3; }
        if (cache_len < 4 && n >= 4) { cache[3] = 4; cache_len = 4; }
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = cache[i-2];                    /* a(i-1)  */
        val += cache[i-4];                         /* + a(i-3) */
        val -= cache[i-5];                         /* - a(i-4) */
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
