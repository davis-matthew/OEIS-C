/*
 * A033770  Product t2(q^d); d | 11, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to x^2 + 11*y^2 = 8*n +
 *            12
 * Range:     u8 n <= 7, u16 n <= 1636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9587 (2026-11-01)
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

/* Computes the n-th term of OEIS sequence A033770. */
seq_t a(idx_t n)
{
    /* S = 8*n + 12 */
    seq_t S = ((seq_t)8) * n + ((seq_t)12);
    /* maximum possible odd y: floor(sqrt(S/11)) */
    seq_t y_max = floor_sqrt(S / ((seq_t)11));

    seq_t count = 0;
    for (seq_t y = 1; y <= y_max; y += 2) {
        /* R = S - 11*y^2 */
        seq_t y2   = y * y;
        seq_t sub  = ((seq_t)11) * y2;
        seq_t R    = S - sub;
        if (R <= (seq_t)0)
            continue;
        seq_t x = floor_sqrt(R);
        if (x * x != R)
            continue;
        /* x is automatically odd because R ≡ 1 (mod 8) */
        if (x & ((seq_t)1))
            ++count;
    }
    return count;
}
