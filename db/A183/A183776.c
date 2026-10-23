/*
 * A183776  Half the number of (n+1) X 4 binary arrays with no 2 X 2 subblock
 *          having exactly 2 ones.
 * https://oeis.org/A183776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 18*a(n-2) - 13*a(n-3) - 70*a(n-4) + 24*a(n-5)
 *            + 64*a(n-6) Initial values: a(0)=8, a(1)=33, a(2)=161, a(3)=730,
 *            a(4)=3435, a(5)=15887, a(6)=74148
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms */
    static idx_t cache_cap = 0;   /* allocated size of cache */

    /* Base values */
    static const seq_t init[7] = {
        (seq_t)8, (seq_t)33, (seq_t)161, (seq_t)730,
        (seq_t)3435, (seq_t)15887, (seq_t)74148
    };

    /* Initial allocation of cache (first 7 terms) */
    if (cache == NULL) {
        cache_cap = 8;                       /* start with capacity 8 */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_len = 7;
    }

    /* Return value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow and compute terms up to index n */
    while (cache_len <= n) {
        /* Ensure enough capacity */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(cache_len) using the recurrence */
        /* pos = 2*a(n-1) + 18*a(n-2) + 24*a(n-5) + 64*a(n-6) */
        seq_t pos = ((seq_t)2) * cache[cache_len - 1];
        pos += ((seq_t)18) * cache[cache_len - 2];
        pos += ((seq_t)24) * cache[cache_len - 5];
        pos += ((seq_t)64) * cache[cache_len - 6];

        /* neg = 13*a(n-3) + 70*a(n-4) */
        seq_t neg = ((seq_t)13) * cache[cache_len - 3];
        neg += ((seq_t)70) * cache[cache_len - 4];

        /* a(n) = pos - neg */
        cache[cache_len] = pos - neg;
        cache_len++;
    }

    return cache[n];
}
