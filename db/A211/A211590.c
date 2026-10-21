/*
 * A211590  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          four, five, six or eight distinct values for every i,j,k<=n.
 * https://oeis.org/A211590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical 14‑term recurrence a(1)=16, a(2)=48, …, a(14)=1549486.
 *            For n>14: a(n)=4*a(n-1)+11*a(n-2)-61*a(n-3)-20*a(n-4)+343*a(n-5)
 *            -156*a(n-6)-890*a(n-7)+756*a(n-8)+1066*a(n-9)
 *            -1196*a(n-10)-504*a(n-11)+720*a(n-12)+72*a(n-13)-144*a(n-14)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined offset, return 0 */

    /* initial 14 values */
    static const seq_t init_vals[14] = {
        (seq_t)16, (seq_t)48, (seq_t)124, (seq_t)306,
        (seq_t)712, (seq_t)1682, (seq_t)3824, (seq_t)9010,
        (seq_t)20556, (seq_t)48942, (seq_t)112920, (seq_t)272310,
        (seq_t)635488, (seq_t)1549486
    };

    static seq_t *cache = NULL;      /* cache for computed values */
    static idx_t cache_n = 0;        /* number of cached entries */

    /* allocate or grow cache to hold at least n entries */
    if ((n > cache_n) || (cache == NULL)) {
        idx_t new_n = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_n);
        if (!new_cache) {
            /* fallback: if allocation fails, reset cache and recompute */
            cache = (seq_t *)malloc(sizeof(seq_t) * new_n);
            new_cache = cache;
        }
        cache = new_cache;
        /* fill newly allocated part */
        for (idx_t i = cache_n; i < new_n; ++i) {
            if (i < 14) {
                cache[i] = init_vals[i];
            } else {
                /* recurrence: a(n) with i == n-1 (0‑based index) */
                cache[i] =
                    (seq_t)4  * cache[i-1]  /* a(n-1) */
                  + (seq_t)11 * cache[i-2]  /* a(n-2) */
                  - (seq_t)61 * cache[i-3]  /* a(n-3) */
                  - (seq_t)20 * cache[i-4]  /* a(n-4) */
                  + (seq_t)343* cache[i-5]  /* a(n-5) */
                  - (seq_t)156* cache[i-6]  /* a(n-6) */
                  - (seq_t)890* cache[i-7]  /* a(n-7) */
                  + (seq_t)756* cache[i-8]  /* a(n-8) */
                  + (seq_t)1066*cache[i-9]  /* a(n-9) */
                  - (seq_t)1196*cache[i-10] /* a(n-10) */
                  - (seq_t)504*cache[i-11]  /* a(n-11) */
                  + (seq_t)720*cache[i-12]  /* a(n-12) */
                  + (seq_t)72 *cache[i-13]  /* a(n-13) */
                  - (seq_t)144*cache[i-14]  /* a(n-14) */
                  ;
            }
        }
        cache_n = new_n;
    }

    return cache[n-1];
}
