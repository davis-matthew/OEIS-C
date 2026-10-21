/*
 * A063401  a(n) = a(n-1)*a(n-2)*a(n-3) with a(0)=1, a(1)=2, a(2)=2.
 * https://oeis.org/A063401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-2) * a(n-3) for n >= 3 initial values: a(0)
 *            = 1, a(1) = 2, a(2) = 2
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL; /* cache of already computed terms (0 .. cache_size-1) */
static idx_t  cache_size = 0;

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* Extend cache to include term n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!new_cache) { /* allocation failure: return 0 (seq_t(0)) */
            /* In compile‑time the "0" literal will be converted to an appropriate seq_t value */
            return (seq_t)0;
        }
        seq_cache = new_cache;

        /* Compute all missing terms in order */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                seq_cache[i] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                seq_cache[i] = (seq_t)2;
            } else {
                seq_cache[i] = seq_cache[i-1] * seq_cache[i-2] * seq_cache[i-3];
            }
        }
        cache_size = new_size;
    }
    return seq_cache[n];
}
