/*
 * A034023  Imprimitively represented by x^2+y^2.
 * https://oeis.org/A034023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer that can be written as x² + y²
 *            with gcd(x, y) > 1 (imprimitive sum of two squares)
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-03)
 * Helpers:   gcd, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static seq_t   next_candidate = 1;

/* Ensure the internal cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();               /* allocation failed */

    /* Copy existing elements. */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Free the old array (safe if cache == NULL). */
    if (cache) free_seq_array(cache);

    cache    = new_cache;
    cache_cap = new_cap;
}

/* Return true if n can be represented as x² + y² with gcd(x, y) > 1. */
static bool is_imprimitive_sum_of_squares(seq_t n)
{
    if (n < (seq_t)0) return false;

    seq_t limit = floor_sqrt(n);
    for (seq_t x = 0; x <= limit; ++x) {
        seq_t x2 = x * x;
        seq_t y2 = n - x2;
        seq_t y  = floor_sqrt(y2);
        if (y * y == y2 && gcd(x, y) > (seq_t)1)
            return true;
    }
    return false;
}

/* Return the n‑th term of the sequence.  Calls may be out of order. */
seq_t a(idx_t n)
{
    /* We store terms in a dynamic array.  The size of n is always far
       below (size_t)-1 so the cast is safe for the supported limits. */
    size_t target = (size_t)n;

    /* Generate terms until we have at least target + 1 elements. */
    while (cache_len <= target) {
        seq_t candidate = next_candidate++;
        if (is_imprimitive_sum_of_squares(candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = candidate;
        }
    }

    return cache[target];
}
