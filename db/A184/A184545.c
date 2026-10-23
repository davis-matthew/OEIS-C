/*
 * A184545  Number of (n+2) X 8 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) * +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * for n >
 *            9 * Initial values (1‑based indexing): * a(1)=415, a(2)=1114,
 *            a(3)=2413, a(4)=4774, a(5)=8989, * a(6)=16345, a(7)=28844,
 *            a(8)=49489, a(9)=82648
 * Range:     u8 none, u16 n <= 8, u32 n <= 53, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (index 0 unused) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t cache_max = 0;    /* largest index with a defined value */

    /* First 9 terms (1‑based index) */
    static const seq_t init[9] = {
        415,   /* a(1) */
        1114,  /* a(2) */
        2413,  /* a(3) */
        4774,  /* a(4) */
        8989,  /* a(5) */
        16345, /* a(6) */
        28844, /* a(7) */
        49489, /* a(8) */
        82648  /* a(9) */
    };

    /* Allocate cache if this is the first call */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 10) cache_cap = 10;          /* at least space for 9 terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */
        memset(cache, 0, cache_cap * sizeof(seq_t));

        /* Initialise first 9 terms */
        for (idx_t i = 1; i <= 9; ++i) cache[i] = init[i - 1];
        cache_max = 9;
    }
    /* Grow cache if needed */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        /* zero new slots */
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If term already computed, return it */
    if (n <= cache_max) return cache[n];

    /* Compute new terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += cache[i - 1] * 9;
        pos += cache[i - 3] * 84;
        pos += cache[i - 5] * 126;
        pos += cache[i - 7] * 36; /* missing term in previous version */
        pos += cache[i - 9];

        /* Negative contributions */
        neg += cache[i - 2] * 36;
        neg += cache[i - 4] * 126;
        neg += cache[i - 6] * 84;
        neg += cache[i - 8] * 9;

        /* Compute a(i) */
        cache[i] = pos - neg;   /* pos >= neg for all i ≥ 10 */
    }

    cache_max = n;
    return cache[n];
}
