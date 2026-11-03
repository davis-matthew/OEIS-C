/*
 * A188318  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=1/sqrt(2), k=4, [
 *          ]=floor.
 * https://oeis.org/A188318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/√2⌋ – 2 – ⌊(n–4)/√2⌋ (with n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 146 terms through n = 7503 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

/* Compute ⌊n/√2⌋ for n ≥ 0.  We use the identity
 * ⌊n/√2⌋ = ⌊⌊n√2⌋ / 2⌋, and floor_sqrt(x) gives ⌊√x⌋.
 */
static seq_t floor_pos_alpha(idx_t n)
{
    /* floor(n√2) = floor(√(2n²)) */
    seq_t sq = floor_sqrt((seq_t)2 * (seq_t)n * (seq_t)n);
    return sq / 2;          /* floor( floor(n√2) / 2 ) = ⌊n/√2⌋ */
}

seq_t a(idx_t n)
{
    seq_t floor_n = floor_pos_alpha(n);

    if (n >= 4) {
        seq_t floor_n4 = floor_pos_alpha(n - 4);
        return floor_n - ((seq_t)2) - floor_n4;
    } else {
        /* for n < 4, floor((n-4)/√2) = -( floor((4-n)/√2) + 1 ) */
        seq_t fp = floor_pos_alpha(4 - n);
        return floor_n - ((seq_t)1) + fp;
    }
}
