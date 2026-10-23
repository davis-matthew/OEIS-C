/*
 * A185771  Number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock sum equal
 *          to any horizontal or vertical neighbor 2 X 2 subblock sum.
 * https://oeis.org/A185771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n‑1) + 96*a(n‑3) + 504*a(n‑4) + 792*a(n‑5) + *
 *            2304*a(n‑6) – 13824*a(n‑8) – 13824*a(n‑9) * with initial values
 *            * a(1)=81, a(2)=558, a(3)=3844, a(4)=26784, a(5)=186624, *
 *            a(6)=1296000, a(7)=9000000, a(8)=62568000, a(9)=434972736. * For
 *            n <= 9 the values are returned directly. * The recu...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (a(1) at cache[0]) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialize with the first 9 terms if not already done */
    if (cache_len == 0) {
        /* capacity larger than 9 to reduce reallocs */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed – return 0 as a safe fallback */

        /* fill base values */
        cache[0] = 81ULL;          /* a(1) */
        cache[1] = 558ULL;         /* a(2) */
        cache[2] = 3844ULL;        /* a(3) */
        cache[3] = 26784ULL;       /* a(4) */
        cache[4] = 186624ULL;      /* a(5) */
        cache[5] = 1296000ULL;     /* a(6) */
        cache[6] = 9000000ULL;     /* a(7) */
        cache[7] = 62568000ULL;    /* a(8) */
        cache[8] = 434972736ULL;   /* a(9) */
        cache_len = 9;
    }

    if (n == 0) return 0;  /* offset 1 – a(0) not defined */

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* ensure capacity for the new terms */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* realloc failed – return 0 as safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i : 0‑based index of a(i+1) */
        seq_t val = 3 * cache[i-1]
                  + 96 * cache[i-3]
                  + 504 * cache[i-4]
                  + 792 * cache[i-5]
                  + 2304 * cache[i-6]
                  - 13824 * cache[i-8]
                  - 13824 * cache[i-9];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n-1];
}
