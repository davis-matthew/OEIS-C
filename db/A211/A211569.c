/*
 * A211569  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 4*a(n-1) + a(n-2) -
 *            21*a(n-3) + 16*a(n-4) + 29*a(n-5) - 34*a(n-6) - 6*a(n-7) +
 *            12*a(n-8) with initial values a(1)=24, a(2)=52, a(3)=94,
 *            a(4)=174, a(5)=304, a(6)=552, a(7)=980, a(8)=1802
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache keeping already computed terms.
       cache[0] is unused; indices are 1-based. */
    static seq_t *cache = NULL;   /* dynamic array of terms */
    static size_t cap      = 0;   /* allocated size (cache indices 0..cap-1) */
    static idx_t  computed = 0;   /* highest n for which cache[n] is valid */

    if (n < 1) return 0;          /* below offset: arbitrary value */

    /* Initialize cache on first call */
    if (cache == NULL) {
        /* reserve indices 0..8 (9 elements) */
        cap = 9;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;    /* allocation failed – silent */
        /* initial eight terms */
        cache[1] = 24ULL;  cache[2] = 52ULL;  cache[3] = 94ULL;
        cache[4] = 174ULL; cache[5] = 304ULL; cache[6] = 552ULL;
        cache[7] = 980ULL; cache[8] = 1802ULL;
        computed = 8;
    }

    /* Ensure the array is large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failed – silent */
        /* zero newly allocated part for safety */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms beyond the already known ones */
    if (n > computed) {
        idx_t start = (computed < 9) ? 9 : computed + 1;
        for (idx_t i = start; i <= n; ++i) {
            /* Split positive and negative contributions to stay in unsigned arithmetic */
            seq_t pos = 4 * cache[i-1]
                      +      cache[i-2]
                      + 16 * cache[i-4]
                      + 29 * cache[i-5]
                      + 12 * cache[i-8];
            seq_t neg = 21 * cache[i-3]
                      + 34 * cache[i-6]
                      +  6 * cache[i-7];
            cache[i] = pos - neg;   /* safe: pos ≥ neg for all n */
        }
        computed = n;
    }

    return cache[n];
}
