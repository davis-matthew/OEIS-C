/*
 * A128922  Numbers simultaneously 10-gonal and centered 10-gonal.
 * https://oeis.org/A128922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(1) = 451 a(n+2) = 322*a(n+1) - a(n) + 130 for n >= 0
 *            (Recurrence found in OEIS entry for A128922) The implementation
 *            memoises computed terms so any call order is efficient
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of terms currently cached (cache_len == 0 means none) */

seq_t a(idx_t n) {
    /* initialise cache for first two terms if necessary */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory: return 0 as fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)451;
    }

    /* extend cache up to required index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory: return 0 */
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* tmp = 322 * a(i-1) */
            seq_t prod = cache[i - 1] * (seq_t)322;
            /* a(i) = 322*a(i-1) - a(i-2) + 130 */
            cache[i] = prod - cache[i - 2] + (seq_t)130;
        }
        cache_len = new_len;
    }

    return cache[n];
}
