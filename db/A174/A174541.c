/*
 * A174541  Baron Münchhausen's Sequence.
 * https://oeis.org/A174541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 0 for n = 1; a(n) = 1 if n is triangular, triangular+1,
 *            or if the n-th triangular number T_n is a perfect square or a
 *            square + 1; otherwise a(n) = 2
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility for integer square root */
#include <stdint.h>
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
#include <stdbool.h>

/* Helper that tests whether a seq_t value is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* 1. Check if n is triangular: 8*n + 1 is a perfect square. */
    seq_t v = (seq_t)8 * (seq_t)n + (seq_t)1;
    if (is_square(v)) return (seq_t)1;

    /* 2. Check if n is triangular + 1: n-1 is triangular. */
    if (is_square((seq_t)8 * (seq_t)(n - 1) + (seq_t)1)) return (seq_t)1;

    /* 3. Compute the n-th triangular number T_n = n*(n+1)/2. */
    seq_t t = ((seq_t)n * ((seq_t)n + (seq_t)1)) / (seq_t)2;

    /* 4. Check if T_n is a perfect square. */
    if (is_square(t)) return (seq_t)1;

    /* 5. Check if T_n is a perfect square + 1. */
    if (is_square(t - (seq_t)1)) return (seq_t)1;

    /* Otherwise a(n) = 2. */
    return (seq_t)2;
}
