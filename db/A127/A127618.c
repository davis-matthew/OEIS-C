/*
 * A127618  Number of walks from (0,0) to (n,n) in the region 0 <= x-y <= 4
 *          with the steps (1,0), (0, 1), (2,0) and (0,2).
 * https://oeis.org/A127618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - 2*a(n-3) for n>=5 with initial
 *            values a(0)=1, a(1)=1, a(2)=5, a(3)=22, a(4)=117
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* array of computed terms            */
static idx_t  cache_upto = (idx_t)-1;/* last index filled in cache         */

/* Compute the n‑th term of A127618.  The function is thread‑unsafe
   but satisfies the required external linkage and memoizes results
   to support arbitrary call order. */
seq_t a(idx_t n)
{
    /* initial values for n = 0..4 */
    static const seq_t init[5] = { 1, 1, 5, 22, 117 };

    /* initialise cache on first call */
    if (cache_upto == (idx_t)-1) {
        /* allocate space for at least the first 5 terms */
        size_t cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* out of memory – caller will receive 0 */
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_upto = 4;
    }

    /* if requested value is already cached, return it */
    if (n <= cache_upto) return cache[n];

    /* grow cache to accommodate up to n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_upto + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;       /* out of memory – return 0 */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1]
                 + (seq_t)6 * cache[i-2]
                 - (seq_t)2 * cache[i-3];
    }
    cache_upto = n;
    return cache[n];
}
