/*
 * A039129  Numbers whose base-10 representation has the same number of 2's
 *          and 4's.
 * https://oeis.org/A039129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_2_and_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth non‑negative integer whose decimal representation
   contains an equal number of digits 2 and 4.
   The function builds a cache of found terms and searches sequentially,
   so it works for arbitrary seq_t, whether a primitive integer or an
   arbitrary‑precision big integer. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_2_and_4.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t next_candidate = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but harmless */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
    }

    /* generate terms until we have at least n */
    while (cache_len < n) {
        if (has_equal_2_and_4(next_candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
#ifdef __cplusplus
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
                delete[] cache;
                cache = tmp;
#else
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
#endif
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
