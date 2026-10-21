/*
 * A230356  Nonsquare numbers n such that digit sum of n = digit sum of square
 *          part of n.
 * https://oeis.org/A230356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   square_part_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search-based implementation of OEIS A230356.
 *  Nonsquare numbers n such that digit_sum(n) == digit_sum(square_part(n))   */

/*  Standard headers:   */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/square_part_uint64.h"

/*  Cache for the computed sequence terms  */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/*  Keeps track of the last integer examined  */
static uint64_t last_checked = 0;

/*  Append a value to the cache, allocating more space if needed  */
static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/*  Integer square root (floor) for 64‑bit values  */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;          /* highest power of four <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/*  Test whether a 64‑bit integer is a perfect square  */
static int is_square_uint64(uint64_t x)
{
    if (x == 0) return 1;
    uint64_t r = isqrt_uint64(x);
    return (__uint128_t)r * r == x;
}

/*  Compute the decimal digit sum of a 64‑bit integer  */
static seq_t digit_sum_uint64(uint64_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/*  Main function required by the framework  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;        /* offset 1 – undefined for 0 */

    /* Generate terms until we have at least n of them  */
    while (cache_len < (size_t)n) {
        ++last_checked;
        uint64_t cand = last_checked;
        if (is_square_uint64(cand)) continue;          /* skip perfect squares */

        seq_t ds_cand = digit_sum_uint64(cand);
        seq_t sq_part = square_part_uint64(cand);
        seq_t ds_sq   = digit_sum_uint64((uint64_t)sq_part);

        if (ds_cand == ds_sq) {
            cache_append((seq_t)cand);
        }
    }
    return cache[n - 1];
}
