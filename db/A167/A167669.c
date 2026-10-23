/*
 * A167669  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*(a(n‑1)+…+a(n‑14)) – 66*a(n‑15) + c(n) * where
 *            c(0)=c(15)=1, c(1)…c(14)=2, otherwise c(n)=0. * The recurrence
 *            comes from the rational G.f. in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of previously computed values   */
    static idx_t  size  = 0;      /* number of valid elements in cache     */
    static idx_t  cap   = 0;      /* allocated capacity of cache           */

    /* Allocate or enlarge the cache to hold indices up to n. */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? (n + 1) : (cap << 1);
        if (new_cap < n + 1) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = size; i <= n; ++i) {
        seq_t sum = 0;

        /* 11 * (a(i-1) + … + a(i-14))  */
        for (int k = 1; k <= 14; ++k) {
            if (i >= (idx_t)k) {
                sum += cache[i - (idx_t)k] * (seq_t)11;
            }
        }

        /* – 66 * a(i-15)  */
        if (i >= 15) {
            sum -= cache[i - 15] * (seq_t)66;
        }

        /* Add the numerator contribution c(i)  */
        if (i == 0 || i == 15) {
            sum += (seq_t)1;
        } else if (i <= 14) {
            sum += (seq_t)2;
        }

        cache[i] = sum;
    }

    size = n + 1;
    return cache[n];
}
