/*
 * A071418  a(1)=0, a(n+1)=(a(n)+n)/2 if a(n)+n is even,
 *          a(n+1)=(3*(a(n)+n)+1)/2 otherwise.
 * https://oeis.org/A071418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0; for n≥1, a(n+1) = (a(n)+n)/2 if a(n)+n is even
 *            (3*(a(n)+n)+1)/2 otherwise
 * Range:     u8 n <= 28, u16 n <= 399, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes values up to the requested index using a simple iterative
   memoisation.  Works for any uint64_t index n (within reasonable
   limits).  O(n) time for the first call to compute all values up to n;
   subsequent calls are O(1). */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed values, index 0 unused    */
static idx_t cache_len = 0;       /* highest n for which cache[n] is valid (0 if empty) */

seq_t a(idx_t n)
{
    if (n < 1)                  /* caller should never request n<1 */
        return 0;

    /* Expand cache if necessary */
    if (n > cache_len) {
        size_t newlen = (size_t)n + 1;   /* plus one for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely in tests */
        cache = tmp;

        /* initialise unused zero element */
        if (cache_len == 0)
            cache[0] = 0;

        /* compute values up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t t = prev + (seq_t)(i - 1);   /* a(i-1) + (i-1) */
            if ((t & 1) == 0)
                cache[i] = t >> 1;           /* (a + n)/2 */
            else
                cache[i] = (t * 3 + 1) >> 1; /* (3*(a + n)+1)/2 */
        }
        cache_len = n;
    }

    return cache[n];
}
