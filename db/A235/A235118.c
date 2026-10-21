/*
 * A235118  Number of independent vertex subsets of the graph obtained by
 *          attaching two pendant edges to each vertex of the ladder graph L_n
 *          (L_n is the 2 X n grid graph; see A235117).
 * https://oeis.org/A235118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=24, a(n)=20*a(n-1)+64*a(n-2) for n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t max_cached = 1;  /* highest index stored in cache */
static size_t cache_cap = 2;  /* allocated capacity of cache */

/*
 * Compute the nth term of sequence A235118.
 * On first call, the cache is initialized with a(0)=1 and a(1)=24.
 * Subsequent calls reuse the cache; the array grows automatically
 * as needed for larger n.  The function works for all builds,
 * whether seq_t is a small unsigned integer type or an arbitrary
 * precision integer class compiled as C++17.
 */
seq_t a(idx_t n)
{
    if (!cache) {
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        max_cached = 1;
    }

    if (n <= max_cached)
        return cache[(size_t)n];

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                    /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms that have not yet been cached */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[(size_t)i] =
            cache[(size_t)(i - 1)] * (seq_t)20 +
            cache[(size_t)(i - 2)] * (seq_t)64;
    }
    max_cached = n;
    return cache[(size_t)n];
}
