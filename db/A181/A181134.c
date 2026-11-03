/*
 * A181134  Sum of 13th powers: a(n) = Sum_{j=0..n} j^13.
 * https://oeis.org/A181134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0}^n j^13
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 28, u128 n <= 682,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 9984 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;       /* cached sequence values */
static idx_t  cache_size = 0;     /* number of entries currently cached */

seq_t a(idx_t n)
{
    /* Ensure the cache is initialized and large enough */
    if (cache_size == 0) {
        cache_size = 1;                      /* we need a[0] */
        cache = mem_new(seq_t, cache_size);
        if (!cache) return 0;                /* on allocation failure return 0 */
        cache[0] = (seq_t)0;                 /* a(0) = 0 */
    }

    if (n >= cache_size) {
        /* Extend the cache to hold up to a(n) */
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) return 0;                  /* on allocation failure return 0 */
        cache = tmp;

        /* Compute missing values using the recurrence a(k) = a(k-1) + k^13 */
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t pow = (seq_t)1;
            for (int p = 0; p < 13; ++p) {
                pow *= (seq_t)k;
            }
            cache[k] = cache[k - 1] + pow;
        }
        cache_size = new_size;
    }

    return cache[n];
}
