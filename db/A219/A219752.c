/*
 * A219752  Expansion of 2*x^4*(1-2*x+x^4)/((1+x)*(1-2*x)^2*(1-x-x^2)).
 * https://oeis.org/A219752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A219753(n). Recurrence (for n ≥ 9): a(n) = 4*a(n-1) -
 *            2*a(n-2) - 7*a(n-3) + 4*a(n-4) + 4*a(n-5). Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=4, a(6)=12,
 *            a(7)=26, a(8)=62
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* 9 initial terms of the sequence. */
static const seq_t init_terms[9] = {
    (seq_t)0,
    (seq_t)0,
    (seq_t)0,
    (seq_t)0,
    (seq_t)2,
    (seq_t)4,
    (seq_t)12,
    (seq_t)26,
    (seq_t)62
};

/* Cache of computed terms.  */
static seq_t *cache        = NULL;   /* array of computed terms          */
static size_t cache_cap    = 0;       /* allocated capacity (in elements) */
static idx_t  cache_max    = (idx_t)-1; /* highest index computed so far    */

/* Memoised computation of a(n).  */
seq_t a(idx_t n)
{
    /* Allocate and initialise the cache on first call. */
    if (cache_cap == 0) {
        /* At least need space for the 9 known terms. */
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 9) init_cap = 9;
        seq_t *tmp = seq_realloc(NULL, 0, init_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = init_cap;

        /* Initialise known terms. */
        for (idx_t i = 0; i < 9 && i < (idx_t)cache_cap; ++i) {
            cache[i] = init_terms[i];
        }
        cache_max = 8;                 /* we have terms 0..8 defined */
    }

    /* If the requested index is already computed, return it. */
    if (n <= cache_max) return cache[n];

    /* Expand the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        cache[k] = 4 * cache[k - 1]
                 - 2 * cache[k - 2]
                 - 7 * cache[k - 3]
                 + 4 * cache[k - 4]
                 + 4 * cache[k - 5];
    }

    cache_max = n;
    return cache[n];
}
