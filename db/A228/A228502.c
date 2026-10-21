/*
 * A228502  Number of n X 4 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically, diagonally or
 *          antidiagonally.
 * https://oeis.org/A228502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3) - 3*a(n-4) Initial values:
 *            a(1)=3, a(2)=5, a(3)=29, a(4)=87
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized linear recurrence implementation */
seq_t a(idx_t n) {
    /* static cache: a_cache[i] holds a(i), 1 <= i <= cache_len */
    static seq_t *a_cache = NULL;
    static idx_t cache_len = 0;

    /* 1-indexed array, ignore a_cache[0] */
    if (n <= 4) {
        /* small constant values returned directly */
        switch (n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)5;
            case 3: return (seq_t)29;
            case 4: return (seq_t)87;
        }
    }

    /* Ensure cache capacity up to n */
    if (cache_len < n) {
        /* allocate or extend the cache */
        size_t new_len = (size_t)n + 1; /* +1 for 1-indexing */
        if (!a_cache) {
            a_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        }
        /* If malloc/realloc failed, abort (undefined in contest context) */
        if (!a_cache) return 0;

        /* Initialize existing part if it wasn't set already */
        if (cache_len == 0) {
            a_cache[1] = (seq_t)3;
            a_cache[2] = (seq_t)5;
            a_cache[3] = (seq_t)29;
            a_cache[4] = (seq_t)87;
        }

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 4) continue; /* already set */
            seq_t t1 = a_cache[i-1];
            seq_t t2 = a_cache[i-2];
            seq_t t3 = a_cache[i-3];
            seq_t t4 = a_cache[i-4];
            a_cache[i] = (seq_t)2 * t1 + (seq_t)7 * t2 - (seq_t)2 * t3 - (seq_t)3 * t4;
        }
        cache_len = n;
    }

    return a_cache[n];
}
