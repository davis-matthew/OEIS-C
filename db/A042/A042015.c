/*
 * A042015  Denominators of continued fraction convergents to sqrt(531).
 * https://oeis.org/A042015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(0)=1, a(1)=23, a(2)=1059, a(3)=24380, a(n) = 1060*a(n-2) -
 *            a(n-4) for n>3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache large enough */
    if (n >= cache_size) {
        /* Allocate or grow cache */
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure: return 0 */
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)23; break;
                case 2: cache[i] = (seq_t)1059; break;
                case 3: cache[i] = (seq_t)24380; break;
                default: cache[i] = (seq_t)(1060 * cache[i-2] - cache[i-4]); break;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
