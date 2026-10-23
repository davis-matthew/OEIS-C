/*
 * A028150  Expansion of 1/((1-4x)(1-7x)(1-9x)(1-11x)).
 * https://oeis.org/A028150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/((1-4x)(1-7x)(1-9x)(1-11x)) *
 *            Computed by successive convolution: first 1/(1-4x), then
 *            1/(1-7x), * then 1/(1-9x), then 1/(1-11x). This uses only
 *            addition and * multiplication by a small positive constant, so
 *            it works for any * unsigned seq_t type
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Alloc and free functions for seq_t arrays are available in utils. */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;     /* array of computed coefficients */
static idx_t cache_max = 0;     /* we have computed up to cache_max */

seq_t a(idx_t n) {
    if (cache == NULL || cache_max < n) {
        /* free old cache if any */
        if (cache) free_seq_array(cache);

        /* allocate array for new cache size */
        size_t len = (size_t)n + 1;
        seq_t *cur = alloc_seq_array(len);
        /* first factor 1/(1-4x) */
        cur[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            cur[i] = cur[i-1] * (seq_t)4;

        /* successive convolutions with bases 7, 9, 11 */
        const idx_t bases[3] = {7, 9, 11};
        for (int b = 0; b < 3; ++b) {
            seq_t *next = alloc_seq_array(len);
            next[0] = cur[0];
            for (idx_t i = 1; i <= n; ++i)
                next[i] = cur[i] + (seq_t)bases[b] * next[i-1];
            free_seq_array(cur);
            cur = next;
        }

        cache = cur;
        cache_max = n;
    }
    return cache[n];
}
