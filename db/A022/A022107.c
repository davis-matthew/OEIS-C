/*
 * A022107  Fibonacci sequence beginning 1, 17.
 * https://oeis.org/A022107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 17
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;   /* array of computed terms          */
    static size_t cache_len  = 0; /* number of terms stored [0 .. cache_len-1] */
    static size_t cache_cap  = 0; /* allocated array capacity          */

    /* first call – initialise cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 2;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(EXIT_FAILURE);           /* defensive */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache_len = 2;
    }

    /* extend cache up to n (if necessary) */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) exit(EXIT_FAILURE);           /* defensive */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[(size_t)i] = cache[(size_t)(i-1)] + cache[(size_t)(i-2)];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
