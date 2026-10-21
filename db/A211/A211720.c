/*
 * A211720  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having four
 *          distinct values for every i,j,k<=n.
 * https://oeis.org/A211720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+6*a(n-2)-6*a(n-3)-10*a(n-4)+10*a(n-5)+4*a(n-6)-4*a(n-7),
 *            a(1)=36, a(2)=50, a(3)=72, a(4)=114, a(5)=184, a(6)=316,
 *            a(7)=544
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 33, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0 */

    /* Base values for n = 1..7 */
    static const seq_t base_vals[7] = {36, 50, 72, 114, 184, 316, 544};

    if (n <= 7) return base_vals[n - 1];

    /* Static memoisation cache */
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static size_t cap   = 0;      /* allocated capacity in terms */
    static size_t sz    = 0;      /* number of terms currently stored */

    /* Initialise cache with the first seven terms */
    if (!cache) {
        cap = 16;                  /* start with a modest capacity */
        #ifdef __cplusplus
        cache = new seq_t[cap];
        #else
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        #endif
        if (!cache) return 0;      /* unlikely allocation failure */
        for (size_t i = 0; i < 7; ++i) cache[i] = base_vals[i];
        sz = 7;
    }

    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp;
        #ifdef __cplusplus
        tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        #else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        #endif
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new terms up to n */
    for (size_t i = sz; i < (size_t)n; ++i) {
        seq_t s = cache[i-1] + 6*cache[i-2];
        s += 10*cache[i-5];
        s += 4*cache[i-6];
        seq_t t = 6*cache[i-3] + 10*cache[i-4] + 4*cache[i-7];
        cache[i] = s - t;
    }
    sz = (size_t)n;

    return cache[n-1];
}
