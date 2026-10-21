/*
 * A084240  a(n) = -5*a(n-1) - 4*a(n-2), a(0)=1, a(1)=0.
 * https://oeis.org/A084240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -5*a(n-1) - 4*a(n-2), a(0)=1, a(1)=0
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow repeated calls in any order   */
    static seq_t *cache = NULL;          /* cache of computed values   */
    static idx_t  cache_len   = 0;       /* allocated size            */
    static idx_t  max_computed = 0;      /* highest index computed     */

    /* Initialise cache with base values if not yet done */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        max_computed = 1;
    }

    /* Ensure the cache is large enough for index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – terminate */
            return 0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to n using the recurrence  */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (- (seq_t)5) * cache[i - 1] - (seq_t)4 * cache[i - 2];
        }
        max_computed = n;
    }

    return cache[n];
}
