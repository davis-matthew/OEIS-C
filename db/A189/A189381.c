/*
 * A189381  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=-1+sqrt(2), t=1+sqrt(2).
 * https://oeis.org/A189381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·r/s⌋ + ⌊n·t/s⌋ with * r = 1, s = -1+√2, t = 1+√2.
 *            * Simplifying: a(n) = 5n + ⌊n·√2⌋ + ⌊2n·√2⌋
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9459 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the generic big‑integer utilities supplied by the build. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t nn = (seq_t)n;

    /* nn² */
    seq_t n2 = seq_mul(nn, nn);

    /* floor(n·√2) = floor(√(2 n²)) */
    seq_t floor_ns2 = floor_sqrt(seq_mul(n2, (seq_t)2));

    /* floor(2n·√2) = floor(√(8 n²)) */
    seq_t floor_2ns2 = floor_sqrt(seq_mul(n2, (seq_t)8));

    /* Return a(n) = 5n + floor(n√2) + floor(2n√2). */
    return (seq_t)((seq_t)5 * nn + floor_ns2 + floor_2ns2);
}
