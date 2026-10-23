/*
 * A032803  Expansion of Sum_{i>=0} q^i*theta_3^i.
 * https://oeis.org/A032803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/(1 - x*Theta3(x))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n >= 1, a(n) = sum_{j=0}^{n-1} t_j * a(n-1-j) */
/* where t_0 = 1, t_j = 2 if j is a perfect square > 0, otherwise 0 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array of computed terms */
static idx_t  cache_size = 0;         /* number of terms currently stored (cache_size == number of valid indices = size) */

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache_size == 0) {
        cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        cache_size = 0;
    }
    if (cache_size <= n) {
        size_t old_size = cache_size;
        cache_size = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
        /* compute new entries */
        for (size_t i = old_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                seq_t sum = cache[i - 1];               /* contribution from t_0 = 1 */
                for (idx_t j = 1; (j * j) <= (idx_t)(i - 1); ++j) {
                    sum += cache[i - 1 - (idx_t)(j * j)] * (seq_t)2;
                }
                cache[i] = sum;
            }
        }
    }
    return cache[n];
}
