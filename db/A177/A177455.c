/*
 * A177455  Partial sums of A079062.
 * https://oeis.org/A177455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} A079062(i) A079062(1)=2; for n>1, A079062(n)
 *            = smallest prime p such that p - A079062(n-1) is a perfect power
 *            a^b, a>1, b>1. Hence a(1)=2, a(2)=13, a(3)=32,
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static seq_t *partial_sums = NULL;
static size_t partial_len = 0;
static size_t partial_cap = 0;
static seq_t last_A079062 = 0;  /* last value of A079062 */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (partial_len >= partial_cap) {
        size_t new_cap = partial_cap ? partial_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_buf = (seq_t *)realloc(partial_sums, new_cap * sizeof(seq_t));
        if (!new_buf) return;          /* out of memory – caller will handle it */
        partial_sums = new_buf;
        partial_cap  = new_cap;
    }
}

/* Test whether n is a perfect power a^b with a>1, b>1 (64‑bit). */
static bool is_perfect_power_uint64(idx_t n)
{
    if (n <= 3) return false;          /* 2^2 = 4 is the smallest perfect power */

    for (uint64_t b = 2; ; ++b) {
        unsigned __int128 pow2 = (unsigned __int128)1 << b;
        if (pow2 > n) break;           /* no larger exponent possible */

        /* find an upper bound for the base */
        uint64_t low = 2, high = 2;
        while (1) {
            unsigned __int128 p = 1;
            for (uint64_t i = 0; i < b; ++i) { p *= high; if (p > n) break; }
            if (p > n) break;
            high <<= 1;
            if (high > n) { high = n; break; }
        }

        /* binary search the base */
        uint64_t lo = low, hi = high;
        while (lo <= hi) {
            uint64_t mid = (lo + hi) >> 1;
            unsigned __int128 p = 1;
            for (uint64_t i = 0; i < b; ++i) { p *= mid; if (p > n) break; }
            if (p == n) return true;
            if (p < n) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, safe return */

    /* initialise cache on first call */
    if (partial_len == 0) {
        ensure_capacity(16);
        partial_sums[0] = (seq_t)2;          /* a(1) = 2 */
        last_A079062 = (seq_t)2;
        partial_len = 1;
    }

    /* generate terms up to the requested index */
    while (partial_len < (size_t)n) {
        idx_t start = (idx_t)last_A079062 + 1;
        idx_t candidate = next_prime(start);
        while (!is_perfect_power_uint64(candidate - (idx_t)last_A079062))
            candidate = next_prime(candidate + 1);

        last_A079062 = (seq_t)candidate;
        seq_t new_sum = partial_sums[partial_len - 1] + (seq_t)candidate;
        ensure_capacity(partial_len + 1);
        partial_sums[partial_len] = new_sum;
        partial_len++;
    }

    return partial_sums[(size_t)n - 1];
}
