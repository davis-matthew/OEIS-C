/*
 * A195170  a(n) = 4*n - floor(n*r), where r=(1+sqrt(5))/2 (the golden ratio).
 * https://oeis.org/A195170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - floor(n*r), r = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9607 (2026-11-01)
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

/* Compute 4*n - floor(n*phi) where phi = (1+sqrt(5))/2 */
seq_t a(idx_t n)
{
    /* Cast n to the sequence type to avoid accidental overflow
       in intermediate 64‑bit operations.  */
    const seq_t n_seq = (seq_t)n;

    /* Compute floor(n*sqrt(5)) as floor( sqrt(5*n^2) ). */
    seq_t n_sq = n_seq * n_seq;
    seq_t five_n_sq = n_sq * (seq_t)5;
    seq_t sqrt_term = floor_sqrt(five_n_sq);

    /* floor(n*phi) = floor( (n + floor(n*sqrt(5))) / 2 )  */
    seq_t floor_n_phi = (n_seq + sqrt_term) / (seq_t)2;

    /* a(n) = 4*n - floor(n*phi) */
    return (n_seq * (seq_t)4) - floor_n_phi;
}
