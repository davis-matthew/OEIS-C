/*
 * A143450  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=7.
 * https://oeis.org/A143450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 79, u64 n <= 162,
 *            u128 n <= 328, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A143450
   Recurrence:
     a(0) = 1, a(1) = 3, … , a(8) = 17
     a(n) = a(n-1) + 2*a(n-8)   for n > 8
   Generates the expansion coefficients of 1/(x^7*(1-x-2*x^8)).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storage */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of computed values */
    static size_t cache_cap = 0;   /* allocated size */

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = (seq_t)2 * (seq_t)i + (seq_t)1;
        } else {
            val = cache[i-1] + (seq_t)2 * cache[i-8];
        }
        cache[i] = val;
    }
    cached_len = (idx_t)n + 1;

    return cache[n];
}
