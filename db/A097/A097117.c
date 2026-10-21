/*
 * A097117  Expansion of (1-x)/((1-x)^2 - 4*x^3).
 * https://oeis.org/A097117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) Initial values: a(0)=1,
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of elements in cache  */

    /* Ensure cache can hold up to n */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failure – abort (unlikely for small n) */
            exit(1);
        }
        cache = new_cache;

        /* Initialise new cells using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 1;
            } else {
                seq_t term = 2 * cache[i - 1] - cache[i - 2] + 4 * cache[i - 3];
                cache[i] = term;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
