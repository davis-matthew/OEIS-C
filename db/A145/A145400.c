/*
 * A145400  Number of 2-factors in P_6 X P_n.
 * https://oeis.org/A145400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 49*a(n-2) - 116*a(n-3) - 363*a(n-4) +
 *            627*a(n-5) + 544*a(n-6) - 1061*a(n-7) + 133*a(n-8) + 264*a(n-9)
 *            - 47*a(n-10) - 26*a(n-11) + 3*a(n-12) + a(n-13) for n>13 initial
 *            values: a(1)=0, a(2)=5, a(3)=9, a(4)=222, a(5)=1140, a(6)=13903,
 *            a(7)=99051, a(8)=972080, a(9)=7826275, a(10)=71...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[14] = {
    0ULL,                           /* dummy index 0 */
    (seq_t)0ULL,                    /* a(1) */
    (seq_t)5ULL,                    /* a(2) */
    (seq_t)9ULL,                    /* a(3) */
    (seq_t)222ULL,                  /* a(4) */
    (seq_t)1140ULL,                 /* a(5) */
    (seq_t)13903ULL,                /* a(6) */
    (seq_t)99051ULL,                /* a(7) */
    (seq_t)972080ULL,               /* a(8) */
    (seq_t)7826275ULL,              /* a(9) */
    (seq_t)71053230ULL,             /* a(10) */
    (seq_t)599141127ULL,            /* a(11) */
    (seq_t)5285091303ULL,           /* a(12) */
    (seq_t)45349095730ULL           /* a(13) */
};

static const int coeffs[13] = {
    5, 49, -116, -363, 627, 544, -1061, 133, 264, -47, -26, 3, 1
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    /* Cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_nmax = 0;      /* highest n computed so far */

    /* Ensure cache capacity for indices up to n */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 14)
        new_cap = 14;                 /* at least up to a(13) */

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;                 /* allocation failure */
        /* initialise the first 13 terms */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_vals[i];
        cache_nmax = 13;
    } else if (cache_nmax + 1 < new_cap) {
        /* resize cache */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                 /* allocation failure */
        cache = tmp;
        /* If cache was previously smaller than 13, initialise missing terms */
        if (cache_nmax < 13) {
            for (idx_t i = cache_nmax + 1; i <= 13; ++i)
                cache[i] = init_vals[i];
        }
        cache_nmax = 13;
    }

    /* If n is within the cached range, return it directly */
    if (n <= cache_nmax)
        return cache[n];

    /* Compute new terms up to n */
    idx_t start = (cache_nmax > 13 ? cache_nmax : 13) + 1;
    for (idx_t k = start; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 13; ++i) {
            idx_t idx = k - (i + 1);          /* a(k-i-1) */
            seq_t val = cache[idx];
            int c = coeffs[i];
            if (c > 0)
                pos += val * (seq_t)c;
            else
                neg += val * (seq_t)(-c);
        }
        cache[k] = pos - neg;                /* pos >= neg always holds */
    }
    cache_nmax = n;
    return cache[n];
}
