/*
 * A045068  Numbers whose base-4 representation contains exactly one 0 and two
 *          3's.
 * https://oeis.org/A045068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑4 representation contains exactly one
 *            0 and two 3's
 * Range:     u8 n <= 20, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_valid_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/is_valid_seq.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t cur = 0;        /* current candidate value */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* offset handling */

    if ((size_t)n <= cache_len) return cache[n-1];

    size_t target = (size_t)n;
    while (cache_len < target) {
        cur++;
        if (is_valid_seq(cur)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cur;
        }
    }
    return cache[n-1];
}
