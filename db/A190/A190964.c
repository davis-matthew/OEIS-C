/*
 * A190964  a(n) = 3*a(n-1) - 10*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 20, i64 n <= 38, i128 n <= 77,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 948 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(1)=1; a(n)=3*a(n-1)-10*a(n-2) */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* initialize cache with the first two terms */
    if (cache_size == 0) {
        size_t init_cap = (n < 8) ? 8 : n + 2;
        cache = mem_new(seq_t, init_cap);
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
        cache_cap = init_cap;
    }

    if ((idx_t)cache_size <= n) {
        if ((idx_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while ((size_t)(n + 1) > new_cap) new_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1] - (seq_t)10 * cache[i - 2];
        }
        cache_size = n + 1;
    }
    return cache[n];
}
