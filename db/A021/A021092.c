/*
 * A021092  Expansion of 1/((1-x)*(1-2*x)*(1-4*x)*(1-10*x)).
 * https://oeis.org/A021092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 84*a(n-2) + 148*a(n-3) - 80*a(n-4), with
 *            initial values a(0)=1, a(1)=17, a(2)=205, a(3)=2205
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache and base values */
    if (cache_len == 0) {
        size_t init = 4;
        cache = (seq_t *)realloc(NULL, init * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure, return 0 as a fallback */

        cache[0] = 1;
        cache[1] = 17;
        cache[2] = 205;
        cache[3] = 2205;

        cache_len = init;         /* we have values for indices 0..3 */
    }

    /* extend cache up to index n if needed */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;

        for (size_t i = cache_len; i <= n; ++i) {
            /* recurrence uses only additions and subtractions that are
               guaranteed to be non‑negative in the correct order. */
            seq_t term = 17 * cache[i-1] + 148 * cache[i-3];
            term -= 84 * cache[i-2];
            term -= 80 * cache[i-4];
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
