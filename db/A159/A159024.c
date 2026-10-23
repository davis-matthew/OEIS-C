/*
 * A159024  a(0)=55; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 55; a(n) = a(n‑1) + floor_sqrt(a(n‑1)) for n > 0 */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of elements stored (max index + 1) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure – return 0 */
        cache[0] = (seq_t)55;                 /* a(0) = 55 */
        cache_size = 1;
    }

    /* if the requested value is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow cache to accommodate indices up to n inclusive */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                 /* allocation failure – return 0 */
    cache = tmp;

    /* compute successive terms starting from the first missing index */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t inc  = floor_sqrt(prev);         /* helper from utils */
        cache[i] = prev + inc;
    }

    cache_size = new_size;
    return cache[n];
}
