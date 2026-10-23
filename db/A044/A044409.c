/*
 * A044409  Numbers n such that string 7,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044409: numbers whose decimal representation contains
   the substring "77" but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static inline bool has77(seq_t x)
{
    seq_t t = x;
    int prev = -1;
    while (t > 0) {
        int d = (int)(t % 10);
        if (prev == 7 && d == 7)
            return true;
        prev = d;
        t /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* behavior below offset is undefined */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_candidate = (seq_t)76;  /* start checking from 77 */

    while (cache_len < (size_t)n) {
        ++last_candidate;

        if (has77(last_candidate) && !has77(last_candidate - (seq_t)1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
