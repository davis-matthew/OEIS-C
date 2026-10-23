/*
 * A170245  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170245
 *
 *  The sequence satisfies the linear recurrence derived from its
 *  rational generating function:
 *
 *      a(0) = 1
 *      For n ≥ 1:
 *          a(n) = c(n) + 42 * (a(n-1)+…+a(n-39)) - 903 * a(n-40)
 *          where
 *              c(0)   = 1
 *              c(1)…c(39) = 2
 *              c(40)  = 1
 *              c(k) = 0 for k > 40
 *
 *  The implementation memoises all values in a static array so that
 *  calls to a(n) are O(n) total, independent of the order of calls.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: computed terms 0 … cache_size-1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t i;

    /* grow cache to include index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = reinterpret_cast<seq_t *>(
            realloc(cache, new_size * sizeof(seq_t)));
#else
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!new_cache) return (seq_t)0;   /* out of memory – should not happen */
        cache = new_cache;

        /* compute terms from the current cache_size up to n */
        for (i = cache_size; i <= n; ++i) {
            seq_t sum39 = 0;
            idx_t k;
            for (k = 1; k <= 39 && i >= k; ++k) sum39 += cache[i - k];

            seq_t prev40 = (i >= 40) ? cache[i - 40] : (seq_t)0;

            seq_t c;
            if (i == 0 || i == 40)         c = (seq_t)1;
            else if (i <= 39)              c = (seq_t)2;
            else                           c = (seq_t)0;

            /* recurrence */
            cache[i] = c + (seq_t)42 * sum39 - (seq_t)903 * prev40;
        }
        cache_size = new_size;
    }

    /* already computed */
    return cache[n];
}
