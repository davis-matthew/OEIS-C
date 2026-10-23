/*
 * A170517  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A170517 satisfies
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 28, and for n ≥ 1 */
   /* a(n) = 26 * (a(n-1)+…+a(n-45)) – 351 * a(n-46) + c(n)      */
   /* where c(0)=c(46)=1, c(1)…c(45)=2, and c(n)=0 for n>46.   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms and auxiliary data */
    static seq_t *cache   = NULL;   /* computed values */
    static size_t  cen     = 0;     /* number of elements stored   */
    static size_t  cap     = 0;     /* current capacity of cache   */
    static seq_t  sum45    = 0;     /* sum of last 45 terms (updated each iteration) */

    /* grow cache if needed */
    if (n >= cen) {
        /* required capacity */
        size_t need = (size_t)n + 1;

        if (need > cap) {
            size_t newcap = cap ? cap * 2 : 64;
            if (newcap < need) newcap = need;

            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap   = newcap;
        }

        /* compute missing terms in order */
        for (size_t i = cen; i <= (size_t)n; ++i) {
            seq_t base = 0;

            if (i == 0 || i == 46)          base = 1;
            else if (i <= 45)                base = 2;
            /* else base stays 0 */

            seq_t val = base + (sum45 * 26);

            if (i >= 46)                     /* subtract 351*a(n-46) */
                val -= cache[i - 46] * 351;

            cache[i] = val;

            /* update running sum of last 45 terms */
            sum45 += val;
            if (i >= 45)
                sum45 -= cache[i - 45];
        }

        cen = need;   /* cache now holds terms up to n */
    }

    return cache[n];
}
