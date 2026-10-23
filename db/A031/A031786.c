/*
 * A031786  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains exactly 18 ones.
 * https://oeis.org/A031786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A031786 by testing each integer for a continued‑fraction
 * period that contains exactly 18 occurrences of the digit 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/* Integer square‑root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t l = 0, r = n, res = 0;
    while (l <= r) {
        idx_t m = (l + r) >> 1;
        __int128 sq = (__int128)m * m;
        if (sq <= n) {
            res = m;
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return res;
}

/* Return true if N is a non‑square whose continued‑fraction period for
 * sqrt(N) contains exactly 18 ones. */
static inline bool has_exactly_18_ones(idx_t N)
{
    if (N < 2) return false;
    idx_t a0 = isqrt_u64(N);
    if (a0 * a0 == N) return false;          /* perfect square */

    idx_t m = 0, d = 1, a = a0;
    int ones = 0;

    while (true) {
        m = d * a - m;
        __int128 mm = (__int128)m * m;
        d = (N - (idx_t)mm) / d;
        a = (a0 + m) / d;

        if (a == 1) ++ones;
        if (a == a0 * (idx_t)2) break;       /* period finished */
    }
    return ones == 18;
}

/* Static cache for the sequence.  It is filled lazily and can be
 * queried in any order. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;                    /* next integer to test */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for non‑positive n */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find the next integer that satisfies the property. */
        do { ++next_k; } while (!has_exactly_18_ones(next_k));

        /* Allocate or enlarge the cache as needed. */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len >= cache_cap) {
            cache_cap <<= 1;                       /* double */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Store the found value. */
        cache[cache_len++] = (seq_t)next_k;
    }

    return cache[(size_t)n - 1];
}
