/*
 * A187339  a(n) = floor((7+sqrt(5))*n/4); complement of A187330.
 * https://oeis.org/A187339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((7+sqrt(5))*n/4)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* First part: floor(7*n/4) */
    seq_t part1 = (n_seq * (seq_t)7) / (seq_t)4;
    /* Remainder of 7*n modulo 4, computed with 64‑bit arithmetic */
    idx_t r1 = ((idx_t)(n % 4) * 3) % 4;  /* 7 ≡ 3 (mod 4) */

    /* Compute floor(n*sqrt(5)) via integer sqrt of 5*n^2 */
    seq_t temp = n_seq * n_seq;
    temp *= (seq_t)5;
    seq_t k = floor_sqrt(temp);

    /* Second part: floor(k/4) where k = floor(n*sqrt(5)) */
    seq_t part2 = k / (seq_t)4;
    /* Remainder of k modulo 4 */
    idx_t r = (idx_t)(k % (seq_t)4);

    /* Carry: add 1 if the fractional parts sum to ≥1,
       which occurs precisely when r1 + r ≥ 4 */
    seq_t carry = ((r1 + r) >= 4) ? (seq_t)1 : (seq_t)0;

    return part1 + part2 + carry;
}
