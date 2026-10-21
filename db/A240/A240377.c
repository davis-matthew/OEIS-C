/*
 * A240377  Number of nX2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+11*a(n-2)-a(n-3)-39*a(n-4)-40*a(n-5)+3*a(n-6)+13*a(n-7)
 *            +64*a(n-8)+162*a(n-9)-220*a(n-10)+122*a(n-11)-38*a(n-12)-82*a(n-13)+60*a(n-14)
 *            for n>15 Initial values: a(1)=2, a(2)=10, a(3)=44, a(4)=148,
 *            a(5)=636, a(6)=2430, a(7)=9648, a(8)=37946, a(9)=149336,
 *            a(10)=588102, a(11)=2315310,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – n>=1 in normal usage */

    /* Static cache for memoisation (1‑based). */
    static seq_t *cache    = NULL; /* array 0..cache_last */
    static idx_t cache_last = 0;   /* last computed index in cache */

    /* Initial values array (index 0 unused). */
    static const seq_t init[16] = {
        0ULL,            /* 0 unused */
        2ULL, 10ULL, 44ULL, 148ULL, 636ULL, 2430ULL,
        9648ULL, 37946ULL, 149336ULL, 588102ULL,
        2315310ULL, 9114874ULL, 35884720ULL,
        141278356ULL, 556180846ULL
    };

    /* Allocate or grow cache to hold indices up to n. */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0;
        cache_last = 0;
    } else if (n > cache_last) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = cache_last + 1; i <= n; ++i)
            cache[i] = 0;              /* initialise new entries */
    }

    /* Return cached value if already computed. */
    if (n <= cache_last)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 15) {                /* use known initial values */
            cache[i] = init[i];
            continue;
        }
        seq_t val = (seq_t)2 * cache[i - 1]          /* 2*a(n-1) */
                  + (seq_t)11 * cache[i - 2]        /* 11*a(n-2) */
                  - cache[i - 3]                   /* -a(n-3) */
                  - (seq_t)39 * cache[i - 4]       /* -39*a(n-4) */
                  - (seq_t)40 * cache[i - 5]       /* -40*a(n-5) */
                  + (seq_t)3  * cache[i - 6]       /* +3*a(n-6) */
                  + (seq_t)13 * cache[i - 7]       /* +13*a(n-7) */
                  + (seq_t)64 * cache[i - 8]       /* +64*a(n-8) */
                  + (seq_t)162* cache[i - 9]       /* +162*a(n-9) */
                  - (seq_t)220* cache[i -10]       /* -220*a(n-10) */
                  + (seq_t)122* cache[i -11]       /* +122*a(n-11) */
                  - (seq_t)38 * cache[i -12]       /* -38*a(n-12) */
                  - (seq_t)82 * cache[i -13]       /* -82*a(n-13) */
                  + (seq_t)60 * cache[i -14];      /* +60*a(n-14) */
        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
