/*
 * A038192  Bisection of A001317.
 * https://oeis.org/A038192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (2 * a(n)) XOR (3 * a(n)), a(0) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first term */
    if (cache == NULL) {
        cache_len = 1;
        cache = mem_new(seq_t, cache_len);
        if (cache == NULL) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)3;
    }

    /* grow cache up to required index */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (tmp == NULL) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];
            seq_t double_a = prev + prev;            /* 2 * prev */
            seq_t triple_a = double_a + prev;        /* 3 * prev */
            cache[i] = double_a ^ triple_a;          /* XOR operation */
        }
        cache_len = new_len;
    }

    return cache[n];
}
