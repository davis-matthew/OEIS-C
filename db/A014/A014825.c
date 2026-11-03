/*
 * A014825  a(n) = 4*a(n-1) + n with n > 1, a(1)=1.
 * https://oeis.org/A014825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 4*a(n-1) + n for n > 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 967 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* seq_t and idx_t are defined externally */
static seq_t *cache = NULL;     /* cache[i] holds a(i) for i >= 1 */
static idx_t cache_len = 0;     /* largest index that is already computed */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;        /* the sequence is defined for n >= 1 only */
    }

    /* Initialise cache and the first term a(1) = 1 */
    if (cache_len == 0) {
        idx_t init_size = (n >= 1) ? (n + 1) : 2;
        cache = mem_new(seq_t, (size_t)init_size);
        if (!cache) {
            return (seq_t)0;     /* out of memory – return zero as sentinel */
        }
        cache[1] = (seq_t)1;
        cache_len = 1;
    }
    /* Grow cache if the requested index is beyond the current size */
    else if (n > cache_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            return (seq_t)0;
        }
        cache = tmp;
    }

    /* Compute any missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = 4 * a(i-1) + i */
        seq_t prev = cache[i - 1];
        seq_t inc  = (seq_t)i;   /* cast the index to seq_t */
        cache[i] = prev * (seq_t)4 + inc;
    }
    cache_len = n;

    return cache[n];
}
