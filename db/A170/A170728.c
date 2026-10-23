/*
 * A170728  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 49: a(n) = 2 + 45 * Σ_{j=0}^{n-1} a(j) For
 *            n = 50: a(50) = 1 + 45 * Σ_{j=1}^{49} a(j) – 1035 * a(0) For n >
 *            50: a(n) = 45 * Σ_{k=1}^{49} a(n-k) – 1035 * a(n-50) (These
 *            values are derived from the generating function of A170728.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t len   = 0;   /* number of elements currently computed */
    static idx_t cap   = 0;   /* allocated capacity of cache     */
    static seq_t total_sum = 0;   /* sum of all cached terms a[0..len-1] */

    /* grow cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (cache) {
            for (idx_t i = 0; i < len; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)1;
        } else if (i <= 49) {
            term = (seq_t)2 + (seq_t)45 * total_sum;
        } else if (i == 50) {
            term = ((seq_t)1
                    + (seq_t)45 * (total_sum - cache[0])
                    - (seq_t)1035 * cache[0]);
        } else {   /* i > 50 */
            seq_t sum49 = 0;
            for (idx_t k = 1; k <= 49; ++k) sum49 += cache[i - k];
            term = (seq_t)45 * sum49 - (seq_t)1035 * cache[i - 50];
        }
        cache[i] = term;
        total_sum += term;
    }
    len = n + 1;

    return cache[n];
}
