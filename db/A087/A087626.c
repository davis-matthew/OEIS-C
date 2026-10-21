/*
 * A087626  Expansion of 2/(1-2x+sqrt(1-4x+4x^3)).
 * https://oeis.org/A087626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the 5‑term linear recurrence
 *            (6+4*n)*a(n)+(-6-4*n)*a(n+1)+(-18-4*n)*a(n+2)+(24+5*n)*a(n+3)+(-6-n)*a(n+4)=0
 *            with initial values a(0)=1, a(1)=2, a(2)=5, a(3)=13
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function is memoised and works for arbitrary calling order.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cap   = 0;       /* allocated capacity          */
    static idx_t len   = 0;       /* number of terms stored     */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)13;  /* a(3) */
        len = 4;
    }

    /* return cached value if already computed */
    if (n < len) return cache[n];

    /* extend cache to at least n+1 entries */
    if (n + 1 > len) {
        if (n + 1 > cap) {               /* grow buffer */
            idx_t newcap = cap * 2 + 4;
            if (newcap < n + 1) newcap = n + 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
            cap   = newcap;
        }

        /* compute new terms iteratively */
        for (idx_t i = len; i <= n; ++i) {
            /* indices of the previous four terms */
            seq_t a0 = cache[i - 4];   /* a(i-4) */
            seq_t a1 = cache[i - 3];   /* a(i-3) */
            seq_t a2 = cache[i - 2];   /* a(i-2) */
            seq_t a3 = cache[i - 1];   /* a(i-1) */

            /* m = i-4 */
            idx_t m = i - 4;

#ifdef __cplusplus
            /* Use ordinary arithmetic on the arbitrary‑precision type */
            seq_t term0 = a0 * (seq_t)(6 + 4*m);
            seq_t term1 = a1 * (seq_t)(6 + 4*m);
            seq_t term2 = a2 * (seq_t)(18 + 4*m);
            seq_t term3 = a3 * (seq_t)(24 + 5*m);

            seq_t num = term0 + term3 - term1 - term2;
            seq_t denom = (seq_t)(i + 2);   /* (i+2) == (6 + m) */
            cache[i] = num / denom;
#else
            /* 64‑bit builds: protect against overflow with __int128 */
            unsigned __int128 term0 = (unsigned __int128)(6 + 4*m) * a0;
            unsigned __int128 term1 = (unsigned __int128)(6 + 4*m) * a1;
            unsigned __int128 term2 = (unsigned __int128)(18 + 4*m) * a2;
            unsigned __int128 term3 = (unsigned __int128)(24 + 5*m) * a3;

            unsigned __int128 num = term0 + term3 - term1 - term2;
            unsigned __int128 denom = (unsigned __int128)(i + 2);   /* (i+2) == (6 + m) */
            cache[i] = (seq_t)(num / denom);
#endif
        }
        len = n + 1;
    }

    return cache[n];
}
