/*
 * A014931  a(1)=1, a(n) = n*16^(n-1) + a(n-1).
 * https://oeis.org/A014931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 * a(n) = n*16^(n-1) + a(n-1) (n >= 2) * This
 *            implementation memoises values in a static array. * pow16 holds
 *            16^(cache_len-1) so that it can be updated in O(1) * for each
 *            new term
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] = a(1), ... */
static idx_t  cache_len = 0;     /* number of cached terms */
static seq_t  pow16 = (seq_t)1;  /* 16^(cache_len-1); when cache_len==0, pow16=1 */

/* Return a(n) for n >= 1; a(0) returns 0 (undefined offset). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset < 1 is undefined */
    /* Grow the cache if needed */
    if (cache_len < n) {
        size_t newlen = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;

        /* If cache was empty, initialise with a(1) = 1 */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
            /* pow16 already equals 1, which is 16^(1-1) */
        }

        /* Build remaining terms up to n */
        while (cache_len < n) {
            /* Update pow16 to 16^(cache_len) before computing a(cache_len+1) */
            pow16 *= (seq_t)16;
            idx_t k = cache_len + 1;                 /* new index (1‑based) */
            seq_t term = (seq_t)k * pow16 + cache[cache_len - 1];
            cache[cache_len] = term;
            cache_len++;
        }
    }
    return cache[n - 1];
}
