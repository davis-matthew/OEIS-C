/*
 * A062322  Factorials of nonsquarefree numbers, or A013929(n)!, (including
 *          1).
 * https://oeis.org/A062322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   * a(n) = (n-th nonsquarefree integer)!, * where the sequence
 *            starts with a(0)=1 (1!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 99 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool is_nonsquarefree(idx_t n) {
    if (n == 1) return true;  /* 1 is included by definition */
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t sq = p * p;
        if (n % sq == 0) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t cur_i = 1;
    static seq_t fact = 1;  /* factorial of cur_i */

    /* initialise cache with a(0) = 1! */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0; /* out of memory (unlikely in tests) */
        cache[0] = fact;            /* a(0) = 1 */
        cache_len = 1;
    }

    while (cache_len <= n) {
        /* advance to next integer */
        ++cur_i;
        fact = fact * (seq_t)cur_i;  /* increment factorial */

        if (is_nonsquarefree(cur_i)) {
            /* expand cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0; /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = fact;
        }
    }

    return cache[n];
}
