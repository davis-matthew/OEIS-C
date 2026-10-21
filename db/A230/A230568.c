/*
 * A230568  Number of nX5 0..4 arrays x(i,j) with each element horizontally or
 *          antidiagonally next to at least one element with value (x(i,j)+1)
 *          mod 5, and upper left element zero.
 * https://oeis.org/A230568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) -560*a(n-2) +1930*a(n-3) -5031*a(n-4)
 *            +56930*a(n-5) * -215108*a(n-6) +99076*a(n-7) +178084*a(n-8)
 *            -100200*a(n-9) for n>10 * Initial values: * a(1)=0, a(2)=0,
 *            a(3)=60, a(4)=2582, a(5)=91084, a(6)=3026090, * a(7)=98987768,
 *            a(8)=3223042422, a(9)=104788977596, a(10)=3405425285562
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Function that returns the nth term of OEIS A230568 */
seq_t a(idx_t n)
{
    /* Handle indexes below 1; behaviour not required for n < 1. */
    if (n < 1)
        return 0;

    /* Static cache of already computed terms. Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t max_n_computed = 0;

    /* Base terms (1-based). Constants are small enough for 64‑bit literal. */
    static const seq_t base_vals[11] = {
        0ULL,                  /* 0 unused, placeholder for index 0 */
        0ULL,                  /* a(1) */
        0ULL,                  /* a(2) */
        60ULL,                 /* a(3) */
        2582ULL,               /* a(4) */
        91084ULL,              /* a(5) */
        3026090ULL,            /* a(6) */
        98987768ULL,           /* a(7) */
        3223042422ULL,         /* a(8) */
        104788977596ULL,       /* a(9) */
        3405425285562ULL       /* a(10) */
    };

    /* Ensure cache contains at least n elements. */
    if (cache == NULL)
    {
        /* Allocate initial storage (at least 10 + 1). */
        idx_t size = (n > 10 ? n : 10);
        cache = (seq_t*)malloc((size + 1) * sizeof(seq_t));
        if (cache == NULL)
            return 0;     /* Allocation failed – unlikely in tests. */
        /* Initialise the base terms up to the smaller of size and 10. */
        for (idx_t i = 1; i <= (size < 10 ? size : 10); ++i)
            cache[i] = base_vals[i];
        max_n_computed = size;
    }
    else if (n > max_n_computed)
    {
        /* Extend cache to accommodate new terms. */
        idx_t new_size = n;
        seq_t *new_cache = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            return 0;     /* Allocation failed – unlikely. */
        cache = new_cache;

        /* Initialise any new positions that lie within the first 10 indices. */
        for (idx_t i = max_n_computed + 1; i <= (new_size < 10 ? new_size : 10); ++i)
            cache[i] = base_vals[i];

        /* Compute terms beyond index 10 using the recurrence. */
        for (idx_t i = (max_n_computed > 10 ? max_n_computed : 10) + 1; i <= n; ++i)
        {
            /* Positives */
            seq_t pos = 0;
            pos += cache[i - 1] * (seq_t)48;
            pos += cache[i - 3] * (seq_t)1930;
            pos += cache[i - 5] * (seq_t)56930;
            pos += cache[i - 7] * (seq_t)99076;
            pos += cache[i - 8] * (seq_t)178084;

            /* Negatives */
            seq_t neg = 0;
            neg += cache[i - 2] * (seq_t)560;
            neg += cache[i - 4] * (seq_t)5031;
            neg += cache[i - 6] * (seq_t)215108;
            neg += cache[i - 9] * (seq_t)100200;

            /* Result: pos - neg (guaranteed non‑negative). */
            cache[i] = pos - neg;
        }

        max_n_computed = n;
    }

    return cache[n];
}
