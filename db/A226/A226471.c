/*
 * A226471  Numbers k such that k^2 XOR triangular(k) is a triangular number.
 *          XOR is the bitwise logical exclusive-or operator.
 * https://oeis.org/A226471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A226471 by enumerating k and checking the
   condition “k^2 XOR triangular(k) is triangular”.                  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;        /* cached k that satisfy the condition */
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* current capacity of the cache   */
static seq_t next_k = 0;           /* next k to evaluate (seq_t for full generality) */

/* Helper: ensure cache can hold at least 'need' elements.            */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        if (new_cap < need) new_cap = need;
        seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
        if (!new_ptr) abort();                 /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Test whether x is a triangular number.  (8*x + 1) must be a perfect square. */
static bool is_triangular(seq_t x)
{
    seq_t d = ((seq_t)8) * x + 1;
    seq_t s = floor_sqrt(d);
    return s * s == d;
}

/* Main sequence function – 1‑based index. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        seq_t tri_k = next_k * (next_k + 1) / 2;    /* T(next_k)   */
        seq_t sq     = next_k * next_k;            /* next_k^2    */
        seq_t val    = sq ^ tri_k;                 /* XOR           */
        if (is_triangular(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_k;
        }
        next_k = next_k + 1;
    }
    return cache[n - 1];
}
