/*
 * A159025  a(0)=71; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 71; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0
 * Range:     u8 n <= 0, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;

seq_t a(idx_t n) {
    /* Allocate or grow cache to cover index n */
    if (n >= seq_cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                     /* allocation failed */
        seq_cache = tmp;

        idx_t old_len = seq_cache_len;          /* length before filling */
        if (old_len == 0) {                     /* initialize first term */
            seq_cache[0] = (seq_t)71;
            old_len = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            seq_t inc  = floor_sqrt(prev);
            seq_cache[i] = prev + inc;
        }
        seq_cache_len = n + 1;
    }
    return seq_cache[n];
}
