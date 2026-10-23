/*
 * A167780  Subsequence of A167708 whose indices are 0 mod 5, that is, a(n) =
 *          A167708(5n+5).
 * https://oeis.org/A167780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 340*a(n+1) - a(n); a(0) = 959, a(1) = 326050
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t cache_len = 0;   /* number of valid elements stored */

    /* If we already have the requested term, return it */
    if (n < cache_len)
        return cache[n];

    /* Need to compute up to index n */
    idx_t req = n + 1;            /* entries needed: 0..n inclusive */

    /* Grow the cache if necessary */
    if (req > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, req * sizeof(seq_t));
        if (!tmp)
            abort(); /* memory allocation failure */
        cache = tmp;

        /* Initialise the first terms if they are missing */
        if (cache_len == 0) {
            cache[0] = (seq_t)959;
            cache_len = 1;
        }
        if (cache_len == 1 && req > 1) {
            cache[1] = (seq_t)326050;
            cache_len = 2;
        }

        /* Compute any remaining missing terms via the linear recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = ((seq_t)340 * cache[i - 1]) - cache[i - 2];
        }

        cache_len = req; /* we now have terms up to n */
    }

    return cache[n];
}
