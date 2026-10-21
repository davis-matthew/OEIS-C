/*
 * A240701  Number of n X 3 0..3 arrays with no element equal to a different
 *          number of vertical neighbors than horizontal neighbors, with new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A240701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) + 40*a(n-2) + 22*a(n-3) - 60*a(n-4) - 40*a(n-5)
 *            - 21*a(n-6) + 45*a(n-7) Initial values: a(1)=2, a(2)=27,
 *            a(3)=461, a(4)=7980, a(5)=138562, a(6)=2406061, a(7)=41780245
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* below offset – undefined */

    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;               /* number of entries stored */

    const idx_t init_count = 7;
    static const seq_t init_vals[7] = {
        (seq_t)2,        /* a(1) */
        (seq_t)27,       /* a(2) */
        (seq_t)461,      /* a(3) */
        (seq_t)7980,     /* a(4) */
        (seq_t)138562,   /* a(5) */
        (seq_t)2406061,  /* a(6) */
        (seq_t)41780245  /* a(7) */
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(init_count * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 0; i < init_count; ++i)
            cache[i] = init_vals[i];
        cache_size = init_count;
    }

    /* if requested term is already computed, return it */
    if (n <= cache_size)
        return cache[n - 1];

    /* grow cache to at least n terms, doubling each time */
    size_t new_cap = cache_size;
    while (new_cap < n)
        new_cap = (new_cap == 0) ? init_count : new_cap << 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;

    /* compute missing terms using the 7‑term recurrence */
    for (size_t i = cache_size; i < n; ++i) {   /* i : index in cache (0‑based) */
        /* terms needed: cache[i-1] … cache[i-7] exist because i >= 7 (since n > cache_size >= 7) */
        seq_t val = (seq_t)45 * cache[i - 7]       /* a(n-7) */
                    + (seq_t)15 * cache[i - 1]     /* a(n-1) */
                    + (seq_t)40 * cache[i - 2]     /* a(n-2) */
                    + (seq_t)22 * cache[i - 3]     /* a(n-3) */
                    - (seq_t)60 * cache[i - 4]     /* a(n-4) */
                    - (seq_t)40 * cache[i - 5]     /* a(n-5) */
                    - (seq_t)21 * cache[i - 6];    /* a(n-6) */
        cache[i] = val;
    }

    cache_size = n;
    return cache[n - 1];
}
