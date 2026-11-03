/*
 * A135412  Integers that equal three times the Heronian mean of two positive
 *          integers.
 * https://oeis.org/A135412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A135412: integers n such that
   n = x + y + sqrt(xy) for some positive integers x,y. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* True if m can be written as u^2 + v^2 + u*v with u,v>=1. */
static bool has_representation(seq_t m)
{
    /* We need to find u such that
       4*m - 3*u^2 is a perfect square.
       Only need u up to sqrt(m). */
    seq_t sqrt_m = floor_sqrt(m);
    idx_t limit = (idx_t)sqrt_m;   /* safe: sqrt_m fits in 64 bits for needed size */
    for (idx_t a = 1; a <= limit; ++a) {
        seq_t u = (seq_t)a;
        seq_t u_sq = u * u;
        seq_t left  = 3 * u_sq;          /* 3*u^2 */
        seq_t right = 4 * m;             /* 4*m */
        if (left > right) continue;     /* delta would be negative */

        seq_t delta = right - left;      /* 4*m - 3*u^2 */

        seq_t rt = floor_sqrt(delta);
        if (rt * rt != delta) continue; /* not a perfect square */

        /* b = (rt - u)/2 must be a positive integer.
           Hence rt > u and (rt - u) even. */
        if (rt <= u) continue;
        seq_t diff = rt - u;
        if ((diff & 1) != 0) continue;
        if (diff == 0) continue;        /* b would be zero */

        /* Found suitable (u,v). */
        return true;
    }
    return false;
}

/* True if n is in the sequence.  We test every divisor of n
   to see whether that divisor can be written as a^2+b^2+ab. */
static bool in_seq_value(seq_t n)
{
    /* Look at each divisor d up to sqrt(n). */
    seq_t sqrt_n = floor_sqrt(n);
    idx_t limit = (idx_t)sqrt_n;   /* holds for the range we need */
    for (idx_t a = 1; a <= limit; ++a) {
        seq_t d = (seq_t)a;
        if (n % d == 0) {
            if (has_representation(d)) return true;
            seq_t other = n / d;
            if (other != d && has_representation(other)) return true;
        }
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined, but safe */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 3;   /* first term is 3 */

    while (cache_len < n) {
        seq_t cand = next_candidate;
        while (!in_seq_value(cand)) {
            cand = cand + 1;
        }

        /* store the found term */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cand;
        next_candidate = cand + 1;
    }
    return cache[n - 1];
}
