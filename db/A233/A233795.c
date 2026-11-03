/*
 * A233795  Number of triangular numbers between triangular(n) and n^2.
 * https://oeis.org/A233795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triangular numbers T(k)=k(k+1)/2 with * T(n) <
 *            T(k) < n². * Given n ≥ 0, the count is k_max – n where * k_max =
 *            floor( (√(1+8n²) – 1)/2 ) if 1+8n² is not a perfect square, *
 *            otherwise k_max = floor( (√(1+8n²) – 1)/2 ) – 1. * The sequence
 *            starts with a(0)=0, a(1)=0, a(2)=0, …
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-01)
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
    /* cast index to seq_t for arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;

    /* discriminant 1 + 8n² */
    seq_t disc     = (seq_t)1 + (seq_t)8 * n_sq;
    seq_t sqrt_disc = floor_sqrt(disc);

    /* candidate maximum k */
    seq_t k_raw = (sqrt_disc - (seq_t)1) / (seq_t)2;

    /* if discriminant is a perfect square, subtract one to keep the
     * inequality strict (k(k+1) < 2n²).   */
    if (sqrt_disc * sqrt_disc == disc)
        if (k_raw > (seq_t)0)
            k_raw--;

    /* answer is max(0, k_raw – n) */
    if (k_raw > nn)
        return k_raw - nn;
    else
        return (seq_t)0;
}
