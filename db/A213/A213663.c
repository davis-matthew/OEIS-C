/*
 * A213663  Number of dominating subsets of the graph G(n) obtained by joining
 *          each vertex of the path graph P_{n+1} on n+1 vertices with an
 *          additional vertex (the join of K_1 and P_{n+1}).
 * https://oeis.org/A213663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 2^(n-2) (n>=4) Initial values:
 *            a(1)=7, a(2)=13, a(3)=25
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"   /* seq_alloc, seq_alloc_new_array */
#include "../utils/seq_realloc.h" /* seq_realloc */
#include "../utils/pow2.h"        /* pow2 */

static seq_t *cache = NULL;     /* 1‑based cache */
static size_t cache_cap = 0;    /* allocated capacity (including index 0) */
static idx_t  computed  = 0;    /* largest n for which cache[n] is valid */

/* make sure the cache can hold indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n>=1 */

    /* ensure we can access cache[n] */
    ensure_size(n);

    /* initialise base terms on first use */
    if (computed == 0) {
        ensure_size(3);                   /* need room for indices 1..3 */
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)25;
        computed = 3;
    }

    /* return already computed value if available */
    if (n <= computed)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t pw = pow2(i-2);             /* 2^(i-2) */
        cache[i] = v1 + v2 + v3 + pw;
    }
    computed = n;
    return cache[n];
}
