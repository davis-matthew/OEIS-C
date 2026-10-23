/*
 * A164599  a(n) = 14*a(n-1) - 47*a(n-2), for n > 1, with a(0) = 1, a(1) = 15.
 * https://oeis.org/A164599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 47*a(n-2), a(0) = 1, a(1) = 15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 15;

    static seq_t *cache      = NULL;   /* memoization array */
    static size_t cache_cap  = 0;       /* allocated size */
    static size_t computed_upto = 0;    /* highest index already computed */

    /* allocate initial cache big enough for the requested index */
    if (!cache) {
        size_t needed = (size_t)n + 1;
        if (needed < 2) needed = 2;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – unlikely in test harness */
        cache[0] = 1;
        cache[1] = 15;
        /* set all other entries to 0 for safety */
        for (size_t i = 2; i < needed; ++i) cache[i] = 0;
        cache_cap = needed;
        computed_upto = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* new elements are implicitly 0 if we want; they will be written below */
    }

    /* compute values up to n (iteratively to avoid deep recursion) */
    for (size_t i = computed_upto + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)14 * cache[i - 1] - (seq_t)47 * cache[i - 2];
    }
    computed_upto = (size_t)n;

    return cache[n];
}
