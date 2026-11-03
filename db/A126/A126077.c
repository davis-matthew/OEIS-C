/*
 * A126077  Numbers such that the sum of the factorials of the digits of the
 *          fourth power is a square.
 * https://oeis.org/A126077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer x such that the sum of the *
 *            factorials of the decimal digits of x⁴ is a perfect square
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 700, u64 n <= 700, u128 n <= 700,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 669 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static const uint64_t fact_d[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL,
    120ULL, 720ULL, 5040ULL, 40320ULL, 362880ULL
};

/* Cache of found terms.  All allocations are performed via malloc/realloc
 * with an explicit cast to (seq_t *), which is valid in both C and C++. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Append a new term to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Check whether a 64‑bit value is a perfect square.  The function floor_sqrt
 * is defined in the utils and works for both simple and arbitrary‑precision
 * types. */
static bool is_perfect_square(uint64_t val)
{
    seq_t r   = floor_sqrt((seq_t)val);
    seq_t sq  = r * r;
    return sq == (seq_t)val;
}

/* Main sequence function.  The cache is filled lazily; each call may
 * re‑search from the last candidate until the n‑th term is found. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 */

    /* Generate terms until we have at least n terms in the cache. */
    while (cache_len < n) {
        idx_t x = next_candidate++;

        /* Compute x⁴ as an unsigned 128‑bit value (fits for the range of
         * interest). */
        unsigned __int128 p = (unsigned __int128)x;
        p = p * p * p * p;         /* p = x⁴ */

        /* Sum the factorials of the decimal digits of p. */
        uint64_t sum = 0;
        if (p == 0) {
            sum += fact_d[0];
        } else {
            while (p) {
                unsigned d = (unsigned)(p % 10);
                sum += fact_d[d];
                p /= 10;
            }
        }

        /* If the sum is a perfect square, record the term. */
        if (is_perfect_square(sum))
            cache_append((seq_t)x);
    }

    /* Return the requested term.  The cache is zero‑indexed. */
    return cache[n - 1];
}
