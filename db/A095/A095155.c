/*
 * A095155  Number of 6-block covers of a labeled n-set.
 * https://oeis.org/A095155
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A095155
   Formula (recurrence): a(n) = 120*a(n-1) - 4593*a(n-2) + 69688*a(n-3)
                               - 428787*a(n-4) + 978768*a(n-5) - 615195*a(n-6)
   Initial values:
      a(3)=7,    a(4)=4977,
      a(5)=711326, a(6)=63602770,
      a(7)=4709047749, a(8)=320401872035
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)     /* before offset – value is irrelevant */
        return (seq_t)0;

    static seq_t *cache = NULL;   /* memoised sequence values                */
    static idx_t cache_cap = 0;    /* allocated size of cache                 */
    static idx_t cache_len = 0;    /* largest index computed + 1 (next free)  */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 9;                         /* indices 0 .. 8 */
        cache = (seq_t *)malloc( cache_cap * sizeof(seq_t) );
        if (!cache) return (seq_t)0;           /* memory failure – unlikely */

        /* fill indices 0 .. 2 (unused) */
        cache[0] = cache[1] = cache[2] = (seq_t)0;

        /* known initial terms – cast to seq_t to allow wrapping on small types */
        cache[3] = (seq_t)7ULL;
        cache[4] = (seq_t)4977ULL;
        cache[5] = (seq_t)711326ULL;
        cache[6] = (seq_t)63602770ULL;
        cache[7] = (seq_t)4709047749ULL;
        cache[8] = (seq_t)320401872035ULL;

        cache_len = 9;
    }

    /* extend cache to at least n+1 entries if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;                     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n];            /* if realloc fails, return what we have */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i-1] * (seq_t)120
                      + cache[i-3] * (seq_t)69688
                      + cache[i-5] * (seq_t)978768
                      - cache[i-2] * (seq_t)4593
                      - cache[i-4] * (seq_t)428787
                      - cache[i-6] * (seq_t)615195;
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
