/*
 * A115535  Numbers k such that the concatenation of k with 4*k gives a
 *          square.
 * https://oeis.org/A115535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that the decimal concatenation of k and 4*k is a perfect square. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_k = 0;

static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, so a(0) is unused */
    while (cache_len < n) {
        /* test next candidate k */
        seq_t k = last_k + 1;
        seq_t t4 = k * 4;                  /* 4*k */
        idx_t d = num_digits(t4);           /* number of decimal digits of 4*k */
        seq_t pow10d = pow10(d);           /* 10^d */
        seq_t concat = k * pow10d + t4;    /* concatenation of k and 4*k */
        seq_t sq = floor_sqrt(concat);     /* floor(sqrt(concat)) */
        if (sq * sq == concat) {
            ensure_capacity();
            cache[cache_len++] = k;
        }
        last_k = k;
    }
    return cache[n - 1];
}
