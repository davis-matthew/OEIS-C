/*
 * A093142  Expansion of g.f. (1-5*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A093142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*10^n + 4) / 9 Recurrence: a(0) = 1, a(n) = 10*a(n-1) -
 *            4 (n > 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 847 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) {
        if (cache_size == 0) {
            cache = mem_new(seq_t, 1);
            cache_size = 1;
            cache[0] = (seq_t)1;
        }
        return cache[0];
    }

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = mem_new(seq_t, new_size);
        if (!new_cache) return (seq_t)0; /* allocation failure */

        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            new_cache[i] = (seq_t)10 * new_cache[i - 1] - (seq_t)4;
        }

        /* free the old cache and update pointers */
        if (cache) mem_free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
