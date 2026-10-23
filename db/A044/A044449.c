/*
 * A044449  Numbers n such that string 1,3 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has13_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044449
   Numbers n such that the base‑4 representation of n contains the substring
   “13” but the base‑4 representation of n+1 does not.
   The implementation enumerates integers sequentially and memoizes all
   terms that satisfy the condition.  This allows arbitrary order of calls
   and guarantees values are computed exactly for all supported seq_t types. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has13_base4.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (n == 0) return 0;      /* offset is 1; undefined for n==0 */

    seq_t cand = last_checked + 1;
    while (cache_len < n) {
        if (has13_base4(cand) && !has13_base4(cand + 1)) {
            /* Ensure capacity for the new element */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_arr = NULL;
#ifdef __cplusplus
                new_arr = new seq_t[new_cap];
                if (cache) {
                    for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
                    delete[] cache;
                }
#else
                new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr && cache) { free(cache); new_arr = NULL; }
#endif
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        last_checked = cand;
        cand++;
    }
    return cache[n - 1];
}
