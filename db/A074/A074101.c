/*
 * A074101  Squares using no prime digit.
 * https://oeis.org/A074101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074101: squares whose decimal digits contain no prime digit (2,3,5,7). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether a decimal representation contains only non‑prime digits */
static inline bool digits_ok(seq_t x)
{
    if (x == 0) return true;             /* 0 contains only the digit 0 */
    while (x) {
        int d = (int)(x % 10);           /* cast safe: result < 10 */
        if (d == 2 || d == 3 || d == 5 || d == 7) return false;
        x /= 10;
    }
    return true;
}

/* dynamic cache of squares that satisfy the digit restriction */
static seq_t *cache     = NULL;   /* pointer to allocated array */
static size_t cache_len = 0;      /* number of elements stored */
static size_t cache_cap = 0;      /* current capacity of the array */
static idx_t  next_k    = 0;      /* next integer k to test (k^2) */

/* enlarge cache storage if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* external linkage as required */
seq_t a(idx_t n)
{
    /* compute until we have at least n qualifying squares */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_k * (seq_t)next_k;
        if (digits_ok(sq)) {
            ensure_cache_capacity();
            cache[cache_len++] = sq;
        }
        ++next_k;
    }
    return cache[n - 1];
}
