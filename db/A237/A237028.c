/*
 * A237028  Number of (n+1) X (1+1) 0..2 arrays with the maximum of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A237028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 83*a(n-3) + 15*a(n-4) + 60*a(n-5) with
 *            a(1)=81, a(2)=563, a(3)=4042, a(4)=30045, a(5)=225731 (offset =
 *            1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of the sequence */
seq_t a(idx_t n)
{
    /* Base values (index starts at 1) */
    static const seq_t base[] = { 0,
                                  81ULL,    /* a(1) */
                                  563ULL,   /* a(2) */
                                  4042ULL,  /* a(3) */
                                  30045ULL, /* a(4) */
                                  225731ULL /* a(5) */
                                };
    /* Very small caches */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* largest n computed, inclusive */
    static size_t cache_cap = 0;  /* allocated size (>= cache_len+1) */

    /* Handle trivial base cases */
    if (n <= 5)
        return base[n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 6;                /* at least 5+1 for 1-indexing */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure: fall back to computation without cache */
            cache_len = 0;
        } else {
            /* set base values */
            for (size_t i = 1; i <= 5; ++i)
                cache[i] = base[i];
            cache_len = 5;
        }
    }

    /* If caching is not active (allocation failed), compute directly */
    if (cache == NULL) {
        seq_t a1 = 81ULL, a2 = 563ULL, a3 = 4042ULL, a4 = 30045ULL, a5 = 225731ULL;
        seq_t res;
        for (idx_t i = 6; i <= n; ++i) {
            seq_t t1 = a5 * 9;          /* a(n-1) * 9 */
            seq_t t3 = a3 * 83;         /* a(n-3) * 83 */
            seq_t t4 = a2 * 15;         /* a(n-4) * 15 */
            seq_t t5 = a1 * 60;         /* a(n-5) * 60 */
            res = t1 + t4 + t5 - t3;    /* combine */
            /* shift for next iteration */
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = res;
        }
        return res;
    }

    /* Resize cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t t1 = cache[i - 1] * 9;
        seq_t t3 = cache[i - 3] * 83;
        seq_t t4 = cache[i - 4] * 15;
        seq_t t5 = cache[i - 5] * 60;
        cache[i] = t1 + t4 + t5 - t3; /* (9*a[i-1] + 15*a[i-4] + 60*a[i-5]) - 83*a[i-3] */
    }
    cache_len = (size_t)n;
    return cache[n];
}
