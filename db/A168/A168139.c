/*
 * A168139  Squares whose sum and number of digits are also squares.
 * https://oeis.org/A168139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   digit_sum, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168139: a(n) = the n-th integer that is a perfect square,
   whose decimal digit sum and number of decimal digits are both perfect
   squares. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper: true if v is a perfect square. */
static bool is_square_seq(seq_t v)
{
    seq_t r = isqrt_seq(v);
    return r * r == v;
}

/* Static cache of discovered terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t   last_m    = 0;   /* next m to examine (m^2) */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined; return 0 for safety. */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Compute (last_m)^2 safely. */
        uint64_t m      = (uint64_t)last_m;
        uint64_t m2     = m * m;                 /* product fits in 64 bits */
        seq_t   sq      = (seq_t)m2;             /* cast to seq_t */

        /* Detect overflow into seq_t (occurs for small seq_t types). */
        if ((uint64_t)sq != m2) {
            /* Further values will also overflow; stop generating. */
            break;
        }

        /* Check that digit sum is a square. */
        seq_t dsum = digit_sum(sq);
        if (!is_square_seq(dsum)) {
            ++last_m;
            continue;
        }

        /* Check that number of digits is a square. */
        int  diglen = num_digits(sq);
        if (!is_square_seq((seq_t)diglen)) {
            ++last_m;
            continue;
        }

        /* All conditions satisfied -- append to cache. */
        if (cache_len == cache_cap) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }
        cache[cache_len++] = sq;
        ++last_m;
    }

    /* If we reached the desired index, return the cached value. */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* If we ran out of representable values before reaching n, return 0. */
    return 0;
}
