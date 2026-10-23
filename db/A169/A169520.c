/*
 * A169520  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A169520:
 *  a(0) = 1
 *  for n >= 1:
 *     a(n) = p_n + 25 * (a(n-1)+a(n-2)+...+a(n-33))
 *            - 325 * a(n-34)
 *  where
 *     p_n = 2  for 1 <= n <= 33
 *          1  for n == 34
 *          0  otherwise
 *  This follows directly from the given generating function.
 *
 *  The function memoises previously computed terms so that any order of
 *  calls to a(idx_t n) works in O(n) total time.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds the computed terms; cache_size is the number of entries */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* cache_size == last_index + 1 */

    /* Extend the cache to accommodate n if necessary */
    if (cache_size <= n) {
        /* allocate larger array */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            /* sum of the previous 33 terms a(i-1) ... a(i-33) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 33; ++k) {
                if (i >= k) sum += cache[i - k];
            }

            /* coefficient for a(i-34) */
            seq_t term34 = (seq_t)0;
            if (i >= 34) term34 = cache[i - 34];

            /* p_i */
            seq_t p = (idx_t)0;
            if (i <= 33)            p = (seq_t)2;
            else if (i == 34)       p = (seq_t)1;

            /* compute a(i) */
            cache[i] = p + (seq_t)25 * sum - (seq_t)325 * term34;
        }

        cache_size = new_size;
    }

    return cache[n];
}
