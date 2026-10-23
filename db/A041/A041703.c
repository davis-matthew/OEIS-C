/*
 * A041703  Denominators of continued fraction convergents to sqrt(371).
 * https://oeis.org/A041703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041703 – Denominators of continued fraction convergents to sqrt(371)
   a(n) = 3390*a(n-6) - a(n-12)   for n > 11
   initial values (n=0..11):
   1, 3, 4, 19, 23, 88, 3367, 10189, 13556, 64413, 77969, 298320
*/
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Persistent cache of all computed terms */
    static seq_t *cache   = NULL;  /* array of seq_t values   */
    static size_t cache_cap = 0;   /* allocated capacity      */
    static size_t cache_len = 0;   /* number of terms filled (>=12 after init) */

    /* Initialise base terms exactly once */
    if (cache_len == 0) {
        const size_t init_cap = 12;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (!cache) return 0; /* out of memory – return 0 */

        cache[0]  = (seq_t)1;    cache[1]  = (seq_t)3;
        cache[2]  = (seq_t)4;    cache[3]  = (seq_t)19;
        cache[4]  = (seq_t)23;   cache[5]  = (seq_t)88;
        cache[6]  = (seq_t)3367; cache[7]  = (seq_t)10189;
        cache[8]  = (seq_t)13556; cache[9] = (seq_t)64413;
        cache[10] = (seq_t)77969; cache[11] = (seq_t)298320;

        cache_cap = init_cap;
        cache_len = init_cap;   /* all 12 positions are now filled */
    }

    /* Grow the array when we need a larger index */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;           /* out of memory – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (including n) */
    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        /* For k < 12 the value already exists */
        if (k >= 12) {
            /* a(k) = 3390*a(k-6) - a(k-12) */
            seq_t term = ( (seq_t)3390 * cache[k - 6] ) - cache[k - 12];
            cache[k] = term;
        }
    }

    if ((size_t)n >= cache_len) {
        cache_len = (size_t)n + 1;   /* remember how many are now filled */
    }

    return cache[(size_t)n];
}
