/*
 * A124701  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 14*a(n-3) + 20*a(n-4) - 24*a(n-5)
 *            - 3*a(n-6) + 6*a(n-7); Initial values: a(0)=1, a(1)=8, a(2)=22,
 *            a(3)=50, a(4)=130, a(5)=338, a(6)=904, a(7)=2444
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t cache_cap = 0;     /* allocated size of cache */
static idx_t cache_len = 0;     /* number of terms currently cached (indices 0 .. cache_len-1) */

/* Expand the cache so that it can hold at least new_cap elements. */
static void expand_cache(idx_t new_cap) {
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure that cache has capacity for indices up to n inclusive. */
static void ensure_cache(idx_t n) {
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap;
        if (new_cap == 0)
            new_cap = 8;
        while (new_cap <= n)
            new_cap <<= 1;
        expand_cache(new_cap);
    }
}

/* Compute a(n) using the linear recurrence and a simple cache. */
seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1,    /* a(0) */
        (seq_t)8,    /* a(1) */
        (seq_t)22,   /* a(2) */
        (seq_t)50,   /* a(3) */
        (seq_t)130,  /* a(4) */
        (seq_t)338,  /* a(5) */
        (seq_t)904,  /* a(6) */
        (seq_t)2444  /* a(7) */
    };

    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            cache[i] =
                (seq_t)8  * cache[i-1]  /* + 8*a(i-1) */
              - (seq_t)21 * cache[i-2]  /* - 21*a(i-2) */
              + (seq_t)14 * cache[i-3]  /* + 14*a(i-3) */
              + (seq_t)20 * cache[i-4]  /* + 20*a(i-4) */
              - (seq_t)24 * cache[i-5]  /* - 24*a(i-5) */
              - (seq_t)3  * cache[i-6]  /* - 3*a(i-6) */
              + (seq_t)6  * cache[i-7]; /* + 6*a(i-7) */
        }
    }
    cache_len = n + 1;   /* we have computed up through index n */

    return cache[n];
}
