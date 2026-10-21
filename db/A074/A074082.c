/*
 * A074082  Coefficient of q^2 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,1).
 * https://oeis.org/A074082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=6, a(6)=16,
 *            a(7)=37. For n≥8: a(n)=3*a(n-1)-5*a(n-3)+3*a(n-5)+a(n-6) (all
 *            terms are non‑negative)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size */
    static idx_t  last  = 0;      /* highest index computed so far */
    static int    ready = 0;      /* initial values set? */

    /* initialise cache with first 8 terms */
    if (!ready) {
        ready = 1;
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 2;
        cache[5] = 6;
        cache[6] = 16;
        cache[7] = 37;
        last = 7;
    }

    /* extend cache if necessary */
    if ((idx_t)n > last) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            /* double capacity up to needed size */
            size_t new_cap = cap;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }
        /* compute new terms using the recurrence */
        for (idx_t k = last + 1; k <= (idx_t)n; ++k) {
            /* recurrence valid only for k >= 8 */
            seq_t t = cache[k-1] * 3;        /* 3*a(k-1) */
            t = t - cache[k-3] * 5;          /* -5*a(k-3) */
            t = t + cache[k-5] * 3;          /* +3*a(k-5) */
            t = t + cache[k-6];              /* +a(k-6)   */
            cache[k] = t;
        }
        last = n;
    }

    return cache[n];
}
