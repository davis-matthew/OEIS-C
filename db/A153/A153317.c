/*
 * A153317  Denominators of continued fraction convergents to sqrt(6/5).
 * https://oeis.org/A153317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 10, for n>1 : if n is even a(n) = 2*a(n-1) +
 *            a(n-2) if n is odd a(n) = 10*a(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------ Memoised cache of computed terms -------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */

    /* If the term is already computed, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to hold all indices up to n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)            /* Allocation failed – abort. */
        abort();
    cache = tmp;

    /* Initialise base terms if they are missing. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (new_len > 1) cache[1] = (seq_t)10;
    } else if (cache_len == 1 && new_len > 1) {
        cache[1] = (seq_t)10;
    }

    /* Compute any new terms that are required. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 1) continue;          /* base values already set */

        if ((i & 1) == 0) {            /* even index: a(2*k) */
            cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2];
        } else {                       /* odd index: a(2*k+1) */
            cache[i] = (seq_t)10 * cache[i - 1] + cache[i - 2];
        }
    }

    cache_len = new_len;
    return cache[n];
}
