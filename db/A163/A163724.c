/*
 * A163724  Number of n X 3 binary arrays with all 1s connected, a path of 1s
 *          from left column to right column, and no 1 having more than two 1s
 *          adjacent.
 * https://oeis.org/A163724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 24*a(n-3) - 10*a(n-4) - 10*a(n-5)
 *            * + 16*a(n-6) - 8*a(n-7) - a(n-8) + 3*a(n-9) - a(n-10) * Initial
 *            values: a(1)=1, a(2)=10, a(3)=48, a(4)=163, a(5)=463, a(6)=1186,
 *            * a(7)=2854, a(8)=6597, a(9)=14839, a(10)=32754, a(11)=71352
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 55, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void add_multiple(seq_t *dest, seq_t src, unsigned int mul)
{
    while (mul--) *dest += src;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1 */

    /* Static cache of computed terms, 1‑based indexing mapped to 0‑based array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of terms currently stored (cache_cap >= n-1 if all needed)

    /* Initial values for n = 1 … 11 */
    static const seq_t init[11] = {
        1ULL,      /* a(1) */
        10ULL,     /* a(2) */
        48ULL,     /* a(3) */
        163ULL,    /* a(4) */
        463ULL,    /* a(5) */
        1186ULL,   /* a(6) */
        2854ULL,   /* a(7) */
        6597ULL,   /* a(8) */
        14839ULL,  /* a(9) */
        32754ULL,  /* a(10) */
        71352ULL   /* a(11) */
    };

    /* Ensure cache is large enough to hold terms up to n */
    if (n > cache_cap)
    {
        /* Extend the cache */
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));

        /* Compute new terms */
        for (idx_t i = (idx_t)(cache_cap + 1); i <= n; ++i)
        {
            size_t idx = (size_t)(i - 1);      /* 0‑based index in cache */

            if (i <= 11)
            {
                cache[idx] = init[i - 1];
                continue;
            }

            /* Accumulate using add_multiple to avoid overflow on small types */
            seq_t pos = 0;
            seq_t neg = 0;

            /* Positive contributions */
            add_multiple(&pos, cache[(size_t)(i - 2)], 7U);   /* 7*a(n-1) */
            add_multiple(&pos, cache[(size_t)(i - 4)], 24U);  /* +24*a(n-3) */
            add_multiple(&pos, cache[(size_t)(i - 7)], 16U);  /* +16*a(n-6) */
            add_multiple(&pos, cache[(size_t)(i - 10)], 3U);  /* +3*a(n-9) */

            /* Negative contributions */
            add_multiple(&neg, cache[(size_t)(i - 3)], 19U);  /* -19*a(n-2) */
            add_multiple(&neg, cache[(size_t)(i - 5)], 10U);  /* -10*a(n-4) */
            add_multiple(&neg, cache[(size_t)(i - 6)], 10U);  /* -10*a(n-5) */
            add_multiple(&neg, cache[(size_t)(i - 8)], 8U);   /* -8*a(n-7) */
            add_multiple(&neg, cache[(size_t)(i - 9)], 1U);   /* -a(n-8)  */
            add_multiple(&neg, cache[(size_t)(i - 11)], 1U);  /* -a(n-10) */

            cache[idx] = pos - neg;  /* final value fits in seq_t by definition */
        }

        cache_cap = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}
