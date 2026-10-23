/*
 * A146126  Number of n X n arrays of squares of integers with every (n-2) X
 *          (n-2) subblock summing to 5 and every element equal to at least
 *          one neighbor.
 * https://oeis.org/A146126
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>12 Initial values: a(4)= 0, a(5)=235, a(6)=9716, a(7)=43201,
 *            a(8)=125064, a(9)=290427, a(10)=583948, a(11)=1060251,
 *            a(12)=1783944
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are provided by the build system. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return zero for indices below offset. */
    if (n < 4)
        return (seq_t)0;

    /* Static cache of computed terms. */
    static seq_t *cache           = NULL;
    static idx_t cache_size       = 0;  /* number of allocated entries */
    static idx_t computed_max     = 3;  /* highest index with a stored value */

    /* Base values for indices 4..12 (stored once). */
    static const seq_t base_vals[13] = {
        (seq_t)0,  /* 0 */
        (seq_t)0,  /* 1 */
        (seq_t)0,  /* 2 */
        (seq_t)0,  /* 3 */
        (seq_t)0,  /* 4 a(4) */
        (seq_t)235,  /* 5 */
        (seq_t)9716, /* 6 */
        (seq_t)43201,/* 7 */
        (seq_t)125064,/* 8 */
        (seq_t)290427,/* 9 */
        (seq_t)583948,/*10 */
        (seq_t)1060251,/*11 */
        (seq_t)1783944 /*12 */
    };

    /* Initialise cache on first call (including base values). */
    if (cache == NULL) {
        cache_size = 13;                     /* indices 0..12 inclusive */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                 /* allocation failure -> zero */

        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = base_vals[i];

        computed_max = 12;
    }
    /* Resize cache if necessary. */
    else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                 /* allocation failure */
        cache = tmp;
        /* initialise new entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Return cached value if already computed. */
    if (n <= computed_max)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* For i <= 12 the value is already in cache; skip. */
        if (i <= 12)
            continue;

        seq_t pos = (seq_t)5 * cache[i - 1]
                  + (seq_t)10 * cache[i - 3]
                  + cache[i - 5];
        seq_t neg = (seq_t)10 * cache[i - 2]
                  + (seq_t)5 * cache[i - 4];
        cache[i] = pos - neg;   /* recurrence is guaranteed non‑negative */
    }
    computed_max = n;
    return cache[n];
}
