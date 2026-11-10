/*
 * A050800  Values of n^2 + 1 resulting from A050796.
 * https://oeis.org/A050800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^2 + 1 where k = A050796(n); A050796 is built by search
 *            (k such that k^2+1 is a sum of two nonzero squares in a
 *            nontrivial way)
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline uint64_t A050796_isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

static bool A050796_is_good(idx_t k)
{
    if (k == 1)
        return true;

    uint64_t m = (uint64_t)k * k + 1;
    uint64_t limit = A050796_isqrt_uint64(m / 2);

    for (uint64_t x = 2; x <= limit; ++x) {
        uint64_t x2 = x * x;
        uint64_t b2 = m - x2;
        uint64_t b  = A050796_isqrt_uint64(b2);
        if (b * b == b2 && b >= x)
            return true;
    }
    return false;
}

static uint64_t *A050796_cache = NULL;
static size_t A050796_len = 0;
static size_t A050796_cap = 0;
static idx_t  A050796_next_k = 1;

static uint64_t A050796_a(idx_t n)
{
    while (A050796_len < n) {
        if (A050796_is_good(A050796_next_k)) {
            if (A050796_cap < A050796_len + 1) {
                size_t new_cap = A050796_cap ? A050796_cap * 2 : 64;
                A050796_cache = (uint64_t *)realloc(A050796_cache, new_cap * sizeof(uint64_t));
                A050796_cap = new_cap;
            }
            A050796_cache[A050796_len++] = (uint64_t)A050796_next_k;
        }
        ++A050796_next_k;
    }
    return A050796_cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t k = (seq_t)A050796_a(n);
    return k * k + 1;
}
