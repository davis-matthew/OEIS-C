/*
 * A091717  Triangular array read by rows: Greatest common divisors of values
 *          of the partition function P: T(n,k) = gcd(P(n), P(k)), 0 <= k < n.
 * https://oeis.org/A091717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 626, u16 n <= 1309, u32 n <= 8362, u64 n <= 11175,
 *            u128 n <= 11175, big any n
 * Validated: standard, pass, 116 terms through n = 8363 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of A091717:  read‑by‑rows of the triangular array
 * T(n,k) = gcd(P(n), P(k)), 0 ≤ k < n.  The function maps a 1‑based
 * index n to the corresponding (row, column) pair, evaluates the
 * required partition numbers, and returns their greatest common divisor.
 * The implementation uses only integer arithmetic and the supplied
 * utilities for array allocation and gcd.  It works for all n ≥ 1
 * regardless of the order of calls and is efficient for n up to about
 * 10⁴ (≈row 141).  For larger n the algorithm remains correct but its
 * running time grows roughly quadratically in the row number. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Find the row r such that triangular number T(r-1) < n ≤ T(r).
 * Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t tri_row(idx_t n)
{
    uint64_t low = 0;
    uint64_t high = 1;

    /* Exponentially increase high until T(high) ≥ n. */
    while ((__uint128_t)high * (high + 1) / 2 < n) {
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid;
        else
            high = mid;
    }
    return (idx_t)high;
}

seq_t a(idx_t n)
{
    /* Determine (row, column) pair from the 1‑based triangular index. */
    idx_t r = tri_row(n);                       /* row number (≥1) */
    __uint128_t past_rows = (__uint128_t)(r - 1) * r / 2; /* T(r-1) */
    idx_t k = (idx_t)(n - (idx_t)past_rows - 1);          /* column (0‑based) */

    /* Compute partition numbers P[0 … r] via dynamic programming. */
    size_t sz = (size_t)(r + 1);
    seq_t *p = alloc_seq_array(sz);

    /* Initialise to zero and set P(0)=1. */
    for (size_t i = 0; i < sz; ++i) p[i] = 0;
    p[0] = 1;

    for (size_t part = 1; part <= r; ++part) {
        for (size_t sum = part; sum <= r; ++sum) {
            p[sum] += p[sum - part];
        }
    }

    /* Compute gcd(P(r), P(k)). */
    seq_t result = gcd(p[(size_t)r], p[(size_t)k]);

    free_seq_array(p);
    return result;
}
