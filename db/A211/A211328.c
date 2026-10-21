/*
 * A211328  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and two, three or four distinct values.
 * https://oeis.org/A211328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n‑1) - 63*a(n‑2) + 130*a(n‑3) - 65*a(n‑4) -
 *            115*a(n‑5) + 69*a(n‑6) + 68*a(n‑7) + 12*a(n‑8) a(1)=24, a(2)=86,
 *            a(3)=310, a(4)=1134, a(5)=4190, a(6)=15582, a(7)=58130,
 *            a(8)=217014
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

#ifdef __cplusplus
/* In C++ use new/delete to construct/ destruct temp big‑int objects. */
#endif

/* Static cache.  Index 0 is unused, indices 1 … last are defined. */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;   /* allocated size (elements) */
static idx_t  cache_last = 0;  /* highest computed index (≥1) */

/* Allocate a new array of seq_t with the required capacity,
   copying any existing values.  Handles C and C++ builds. */
static seq_t *grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return cache;

    seq_t *tmp;
#ifdef __cplusplus
    tmp = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_cap; ++i)
        tmp[i] = cache[i];
    delete [] cache;
#else
    tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return NULL;
#endif
    cache = tmp;
    cache_cap = new_cap;
    return cache;
}

/* Computes the n‑th term of A211328. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of bounds (unused) */

    /* First call: initialise cache with the 8 known terms. */
    if (cache == NULL) {
        /* allocate space for indices 0 … 8 (9 elements) */
        if (!grow_cache(9))
            return 0;                    /* allocation failed */
        cache[0] = 0;                     /* unused dummy value */
        cache[1] = 24;
        cache[2] = 86;
        cache[3] = 310;
        cache[4] = 1134;
        cache[5] = 4190;
        cache[6] = 15582;
        cache[7] = 58130;
        cache[8] = 217014;
        cache_last = 8;
    }

    /* If already computed, return the cached value. */
    if (n <= cache_last)
        return cache[n];

    /* Grow the cache to hold up to index n (need n+1 elements). */
    if (!grow_cache(n + 1))
        return 0;                         /* allocation failed */

    /* Compute terms from cache_last+1 up to n.  The recurrence
       is valid for indices ≥9; indices 1…8 are already initialized. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = ( seq_t )13 * cache[i - 1]
                  - ( seq_t )63 * cache[i - 2]
                  + ( seq_t )130 * cache[i - 3]
                  - ( seq_t )65 * cache[i - 4]
                  - ( seq_t )115 * cache[i - 5]
                  + ( seq_t )69  * cache[i - 6]
                  + ( seq_t )68  * cache[i - 7]
                  + ( seq_t )12  * cache[i - 8];
    }

    cache_last = n;
    return cache[n];
}
