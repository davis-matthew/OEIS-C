/*
 * A170541  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170541:
   a(0) = 1
   for 1 ≤ n ≤ 46: a(n) = 2 + 2·∑_{i=0}^{n-1} a(i)
   a(47) = 2·∑_{i=1}^{46} a(i) – 2
   for n ≥ 48:  a(n) = 2·∑_{i=n-46}^{n-1} a(i) – 3·a(n-47)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values and their cumulative sums */
    static seq_t *cache     = NULL;      /* a[n] values   */
    static seq_t *cum_cache = NULL;      /* cumulative sums: cum[n] = sum_{i=0}^{n} a[i] */
    static size_t cachelen  = 0;          /* number of entries currently stored       */
    static size_t cachecap  = 0;          /* allocated capacity                       */

    /* enlarge caches if necessary */
    if (cachelen <= n) {
        /* determine new capacity */
        size_t newcap = cachecap ? cachecap * 2 : 1;
        while (newcap <= n) newcap *= 2;

        /* reallocate cache arrays */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;

        seq_t *tmp2 = (seq_t *)realloc(cum_cache, newcap * sizeof(seq_t));
        if (!tmp2) abort();
        cum_cache = tmp2;
        cachecap = newcap;

        /* compute new entries */
        for (idx_t i = cachelen; i <= n; ++i) {
            seq_t val;

            if (i == 0) {                /* a(0) = 1 */
                val = (seq_t)1;
                cum_cache[i] = val;
            }
            else if (i <= 46) {           /* a(n) = 2 + 2*sum_{0}^{n-1} a(i) */
                val = (seq_t)2 + (seq_t)2 * cum_cache[i - 1];
                cum_cache[i] = cum_cache[i - 1] + val;
            }
            else if (i == 47) {           /* a(47) = 2*sum_{1}^{46} a(i) – 2 */
                seq_t sum = cum_cache[46] - cum_cache[0];  /* sum a[1..46] */
                val = (seq_t)2 * sum - (seq_t)2;
                cum_cache[i] = cum_cache[i - 1] + val;
            }
            else {                        /* a(n) = 2*sum_{n-46}^{n-1} a(i) – 3*a(n-47) */
                seq_t sum_last = cum_cache[i - 1] - cum_cache[i - 47];
                val = (seq_t)2 * sum_last - (seq_t)3 * cache[i - 47];
                cum_cache[i] = cum_cache[i - 1] + val;
            }
            cache[i] = val;
        }
        cachelen = n + 1;
    }

    return cache[n];
}
