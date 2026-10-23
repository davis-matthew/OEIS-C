/*
 * A043952  Numbers k such that 3 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 3 and 6 occur juxtaposed in the base-8
 *            representation of k but not of k+1
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool has_adj(seq_t x)
{
    seq_t y = x;
    uint8_t prev = 0;
    bool first = true;
    while (y != (seq_t)0) {
        uint8_t d = (uint8_t)(y % (seq_t)8u);
        y /= (seq_t)8u;
        if (!first) {
            if ((prev == 6u && d == 3u) || (prev == 3u && d == 6u))
                return true;
        }
        prev = d;
        first = false;
    }
    return false;
}

static seq_t *cache  = NULL;
static size_t  cache_cap = 0;
static size_t  cache_len = 0;
static seq_t   next_k   = (seq_t)0;

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        seq_t k = next_k;
        while (true) {
            if (has_adj(k) && !has_adj(k + (seq_t)1))
                break;
            k += (seq_t)1;
        }
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = k;
        next_k = k + (seq_t)1;
    }

    return cache[(size_t)(n - (idx_t)1)];
}
