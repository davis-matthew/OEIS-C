/*
 * A228685  Number of 4 X n binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) – 7*a(n-3) – 18*a(n-4) Initial
 *            values: a(1)=16, a(2)=41, a(3)=313, a(4)=1152
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *alloc_cache(size_t cap);
static seq_t *cache   = NULL;   /* cache[0] unused */
static idx_t  cache_high = 0;    /* largest index already computed */
static size_t cache_cap  = 0;    /* allocated capacity (>= cache_high+1) */

/* allocate an array of seq_t of given length */
static seq_t *alloc_cache(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* free a heap‑allocated cache */
static void free_cache(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of range, undefined for the seq */

    /* first time: allocate and initialise base cases */
    if (!cache) {
        cache_cap = (size_t)(n + 1 > 5 ? n + 1 : 5);
        cache = alloc_cache(cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failed */

        cache[1] = (seq_t)16;
        cache[2] = (seq_t)41;
        cache[3] = (seq_t)313;
        cache[4] = (seq_t)1152;
        cache_high = 4;
    }
    /* grow cache if necessary */
    else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1 > 5 ? n + 1 : 5);
        seq_t *new_cache = alloc_cache(new_cap);
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        for (idx_t i = 0; i <= cache_high; ++i)
            new_cache[i] = cache[i];
        free_cache(cache);
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* if requested term is already computed, return it */
    if (n <= cache_high) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_high + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = a1 * ((seq_t)2) + a2 * ((seq_t)16)
                 - a3 * ((seq_t)7) - a4 * ((seq_t)18);
    }
    cache_high = n;

    return cache[n];
}
