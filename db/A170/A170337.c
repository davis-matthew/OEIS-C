/*
 * A170337  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) satisfies the linear recurrence
   a(n) = 38*a(n-1)+38*a(n-2)+ … + 38*a(n-41) – 741*a(n-42)   for n ≥ 42
   with initial values determined by the numerator:
   a(0)=1,  a(1)=40,  a(2)=1560, …, a(41)=19315229942699223188760.
   (p_n = 1 for n=0 or 42, p_n = 2 for 1 ≤ n ≤ 41)
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache containing all computed terms up to the largest request */
    static seq_t cache[10001];
    static size_t computed_len = 0;    /* number of already computed elements */

    /* Initialisation: compute terms up to the requested index n */
    if (n >= computed_len) {
        /* if no terms have been computed yet, set a(0) */
        if (computed_len == 0) {
            cache[0] = (seq_t)1;        /* a(0) = 1 */
            computed_len = 1;
        }

        /* compute subsequent terms sequentially */
        for (idx_t i = (idx_t)computed_len; i <= n; ++i) {
            seq_t sumPrev = (seq_t)0;

            /* sum of the previous up to 41 terms */
            for (idx_t k = 1; k <= 41 && k <= i; ++k)
                sumPrev += cache[i - k];

            seq_t val = (seq_t)38 * sumPrev;

            /* subtract the 42‑term contribution when available */
            if (i >= 42)
                val -= (seq_t)741 * cache[i - 42];

            /* add the numerator contribution for indices ≤ 42 */
            if (i <= 42) {
                seq_t add = (i == 0 || i == 42) ? (seq_t)1 : (seq_t)2;
                val += add;
            }

            cache[i] = val;
        }

        computed_len = (size_t)(n + 1);
    }

    return cache[n];
}
