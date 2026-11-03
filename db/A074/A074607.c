/*
 * A074607  a(n) = 3^n + 6^n.
 * https://oeis.org/A074607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 18*a(n-2), with a(0)=2, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 199 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of cached elements */

seq_t a(idx_t n)
{
    /* Ensure space for indices 0..n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp;
        if (cache == NULL) {
            tmp = mem_new(seq_t, new_size);
            if (!tmp) exit(1);
            cache = tmp;
            cache_size = 0;
        } else {
            tmp = mem_resize(seq_t, cache, new_size);
            if (!tmp) exit(1);
            cache = tmp;
        }
        /* Compute only new entries */
        for (idx_t i = cache_size; i < (idx_t)new_size; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)2;
            } else if (i == 1) {
                cache[1] = (seq_t)9;
            } else {
                cache[i] = cache[i-1] * (seq_t)9 - cache[i-2] * (seq_t)18;
            }
        }
        cache_size = (idx_t)new_size;
    }
    return cache[n];
}
