/*
 * A099156  a(n) = 2^(n-1)*ChebyshevU(n-1, 2).
 * https://oeis.org/A099156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function returns the n‑th term of OEIS A099156.
   It uses a static cache so repeated calls with arbitrary n
   are processed in O(max(n)) time.  The recurrence is applied
   only up to the requested index. */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;   /* cache[i] = a(i) */
    static idx_t  cache_len = 0;  /* cache holds indices 0 .. cache_len-1 */

    /* initialise cache with a(0)=0 and a(1)=1 */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) { /* allocation failed – abort */
            exit(1);
        }
        cache[0] = 0;
        cache[1] = 1;
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
        if (!new_cache) {
            free(cache);
            exit(1);
        }
        cache = new_cache;

        /* compute terms from old_len up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            cache[i] = 8 * cache[i - 1] - 4 * cache[i - 2];
        }
    }

    return cache[n];
}
