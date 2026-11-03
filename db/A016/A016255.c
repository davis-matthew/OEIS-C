/*
 * A016255  Expansion of 1/((1-x)(1-7x)(1-12x)).
 * https://oeis.org/A016255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 84*a(n-2) + 1, a(0) = 1, a(1) = 20
 *            (Recurrence of the expansion of 1/((1-x)(1-7x)(1-12x)).)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (cache == NULL) {
        size = 2;
        cache = mem_new(seq_t, size);
        if (cache == NULL)
            return (seq_t)0;        /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
    }

    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (tmp == NULL)
            return (seq_t)0;        /* allocation failure */
        cache = tmp;
        for (idx_t i = size; i <= n; ++i) {
            cache[i] = (seq_t)19 * cache[i-1] - (seq_t)84 * cache[i-2] + (seq_t)1;
        }
        size = new_size;
    }

    return cache[n];
}
