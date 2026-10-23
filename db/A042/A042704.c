/*
 * A042704  Numerators of continued fraction convergents to sqrt(882).
 * https://oeis.org/A042704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042704: Numerators of continued fraction convergents to sqrt(882)
   Recurrence: a(n) = 39202*a(n-8) – a(n-16)   (n ≥ 16)
   Initial values (n = 0 … 15):
   29, 30, 89, 297, 1871, 5910, 13691, 19601,
   1150549, 1170150, 3490849, 11642697, 73347031,
   231683790, 536714611, 768398401
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The 16 known initial terms */
    static const seq_t init[16] = {
        29U, 30U, 89U, 297U, 1871U, 5910U,
        13691U, 19601U, 1150549U, 1170150U,
        3490849U, 11642697U, 73347031U, 231683790U,
        536714611U, 768398401U
    };

    /* Cached values – grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* For indices below the offset, return the initial terms directly */
    if (n < 16)
        return init[(size_t)n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)cache_len);
        if (!cache) return 0;           /* allocation failed – return 0 */
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init[i];
    }

    /* Grow cache to include requested index if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache,
                            sizeof(seq_t) * (size_t)new_len);
        if (!new_cache) return 0;       /* allocation failed – return 0 */
        cache = new_cache;
        /* Compute missing terms only once, consecutively */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t prod = (seq_t)39202 * cache[k - 8];
            cache[k] = prod - cache[k - 16];
        }
        cache_len = new_len;
    }

    return cache[n];
}
