/*
 * A024569  [ 1/{n*r} ], where r = (1 + sqrt(5))/2 and {x} := x - [ x ].
 * https://oeis.org/A024569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 110 terms through n = 990 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024569: a(n) = floor(1/{n*φ}) with φ = (1+√5)/2
 *
 * Let s = ⌊√(5·n²)⌋.  Then  n·φ = (n + s + ε)/2   with 0 < ε < 1.
 * The fractional part of n·φ is (ε + p)/2, where p = (n + s) mod 2.
 *
 * For p = 1 the fractional part is ≥ 0.5 and a(n) = ⌊2/(1+ε)⌋ = 1.
 * For p = 0 the fractional part is ε/2.  Using
 *
 *   5·n² – s² = ε(2s + ε),
 *
 * we obtain
 *
 *   a(n) = ⌊2/ε⌋ = ⌊(4s)/Δ⌋   with  Δ = 5·n² – s²  (Δ > 0).
 *
 * This formula uses only integer arithmetic and fits the
 * constraints of the evaluation environment.
 */
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) – the formula works for all positive n. */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n²                     */
    seq_t five_n_sq = (seq_t)5 * n_sq;         /* 5·n²                   */
    seq_t s = floor_sqrt(five_n_sq);           /* s = ⌊√(5·n²)⌋             */
    /* parity p = (n + s) mod 2 */
    idx_t parity = ((idx_t)n + (idx_t)s) & 1UL;
    if (parity) {
        return (seq_t)1;                      /* a(n) = 1 when p = 1      */
    }
    /* Δ = 5·n² – s² */
    seq_t delta = five_n_sq - s * s;           /* Δ > 0                    */
    /* a(n) = ⌊4·s / Δ⌋  (integer division truncates toward 0) */
    return (seq_t)((seq_t)4 * s) / delta;
}
