/*
 * A048902  Indices of heptagonal numbers (A000566) which are also hexagonal.
 * https://oeis.org/A048902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 322*a(n-1) - a(n-2) - 96, with a(1)=1, a(2)=221
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL; /* pointer to array of seq_t */
    static idx_t  cache_up_to = 0; /* highest index already computed (n) */
    static size_t cache_cap   = 0; /* allocated capacity (>= cache_up_to) */

    if (n == (idx_t)0) {
        /* sequence is defined only for n >= 1; return 0 for safety */
        return (seq_t)0;
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; for robustness return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* compute new terms up to n, using the recurrence */
    for (idx_t i = cache_up_to + (idx_t)1; i <= n; ++i) {
        if (i == (idx_t)1) {
            cache[0] = (seq_t)1;
        } else if (i == (idx_t)2) {
            cache[1] = (seq_t)221;
        } else {
            cache[i - (idx_t)1] =
                ((seq_t)322 * cache[i - (idx_t)2]) - cache[i - (idx_t)3] - (seq_t)96;
        }
    }

    cache_up_to = n;
    return cache[n - (idx_t)1];
}
