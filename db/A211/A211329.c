/*
 * A211329  Number of (n+1) X (n+1) -5..5 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or two distinct values.
 * https://oeis.org/A211329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15, 19, 25, …; recurrence: a(1) = 15, a(2) = 19, a(3) =
 *            25, a(n) = 2*a(n-1) – a(n-3) for n ≥ 4 (offset 1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_nmax = 0;  /* number of entries stored (a(1)..a(cache_nmax)) */
    static size_t cache_capacity = 0; /* array capacity in entries */

    if (n == 0) return 0; /* undefined but harmless */

    /* Expand cache if necessary */
    if (n > cache_nmax) {
        /* grow capacity until it can hold n entries */
        size_t needed = (size_t)n;
        if (needed > cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity : 4;
            while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
            seq_t *new_cache = new seq_t[new_cap];
#else
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
            /* copy existing entries */
            for (idx_t i = 0; i < cache_nmax; ++i)
                new_cache[i] = cache[i];

            /* free old storage */
            if (cache) {
#ifdef __cplusplus
                delete[] cache;
#else
                free(cache);
#endif
            }
            cache = new_cache;
            cache_capacity = new_cap;
        }

        /* compute missing values */
        for (idx_t i = cache_nmax + 1; i <= n; ++i) { /* i is 1‑based */
            idx_t idx = i - 1; /* 0‑based index */
            if (i == 1)
                cache[idx] = (seq_t)15;
            else if (i == 2)
                cache[idx] = (seq_t)19;
            else if (i == 3)
                cache[idx] = (seq_t)25;
            else {
                /* a(i) = 2*a(i-1) – a(i-3) */
                seq_t v1 = cache[idx - 1];         /* a(i-1) */
                seq_t v3 = cache[idx - 3];         /* a(i-3) */
                cache[idx] = (seq_t)2 * v1 - v3;
            }
        }
        cache_nmax = n;
    }

    return cache[n - 1];
}
