/*
 * A215784  Number of permutations of 0..floor((n*6-1)/2) on even squares of
 *          an n X 6 array such that each row, column, diagonal and
 *          (downwards) antidiagonal of even squares is increasing.
 * https://oeis.org/A215784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-2) - 3*a(n-4), a(1)=1, a(2)=2, a(3)=12, a(4)=29
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses lazy memoization so repeated calls are efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert to size_t for array indexing (n >= 1). */
    size_t idx = (size_t)n;

    /* Lazy static cache: indices 0..computed are stored. */
    static seq_t *cache = NULL;       /* array of cached values */
    static size_t cache_cap = 0;      /* allocated capacity ( +1 for index 0) */
    static size_t computed = 0;       /* highest index that has been computed */

    /* Initial allocation and base values. */
    if (!cache) {
        cache_cap = 5;                /* reserve indices 0..4 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;         /* unlikely but defensive */

        cache[0] = 0;                  /* unused */
        cache[1] = 1;                  /* a(1) */
        cache[2] = 2;                  /* a(2) */
        cache[3] = 12;                 /* a(3) */
        cache[4] = 29;                 /* a(4) */
        computed = 4;
    }

    /* If the requested term is already in the cache, return it. */
    if (idx <= computed) {
        return cache[idx];
    }

    /* Expand the cache as needed. */
    if (idx + 1 > cache_cap) {
        /* Grow by at least 10% to amortize future reallocations. */
        size_t new_cap = cache_cap * 3 / 2 + 1;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;           /* allocation failure handled conservatively */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from computed+1 up to idx using the recurrence. */
    for (size_t i = computed + 1; i <= idx; ++i) {
        /* For i < 5, base values are already present; only i >= 5 requires recurrence. */
        if (i < 5) {
            /* base cases are already in cache; nothing to do */
            continue;
        }
        /* a(i) = 16*a(i-2) - 3*a(i-4)  (uses seq_t arithmetic) */
        seq_t mul16 = cache[i - 2] * (seq_t)16;
        seq_t mul3  = cache[i - 4] * (seq_t)3;
        cache[i] = mul16 - mul3;
    }
    computed = idx;
    return cache[idx];
}
