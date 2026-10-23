/*
 * A170516  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=27, a(2)=702, ... computed by the linear recurrence
 *            * derived from the rational generating function * A(t) = (t^46 +
 *            2 t^45 + 2 t^44 + … + 2 t + 1) *
 *            ------------------------------------------------- * 325 t^46 -
 *            25 t^45 - 25 t^44 - … - 25 t + 1 * The recurrence is * a[n] =
 *            N[n] + 25...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate cache on first call or extend it if required */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache[0] = (seq_t)1;                    /* a(0) */
        cache_size = 1;
    }
    if (n >= cache_size) {
        /* Extend cache to hold a[0..n] */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        cache = tmp;
        /* Compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Compute N[i] */
            seq_t Ni;
            if (i == 0)       Ni = (seq_t)1;
            else if (i <= 45) Ni = (seq_t)2;
            else if (i == 46) Ni = (seq_t)1;
            else              Ni = (seq_t)0;

            /* Sum of up to the last 45 terms */
            seq_t sum = (seq_t)0;
            idx_t maxk = i < 45 ? i : 45;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += cache[i - k];

            /* Recurrence */
            seq_t val = Ni + (seq_t)25 * sum;
            if (i >= 46)
                val -= (seq_t)325 * cache[i - 46];
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
