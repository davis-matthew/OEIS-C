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
 * Helpers:   is_nonsquarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_nonsquarefree.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t cur_i = 1;
    static seq_t fact = 1;  /* factorial of cur_i */

    /* initialise cache with a(0) = 1! */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
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
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = fact;
        }
    }

    return cache[n];
}
