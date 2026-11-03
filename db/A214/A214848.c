/*
 * A214848  First difference of A022846.
 * https://oeis.org/A214848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*sqrt(2)+1/2) - floor(n*sqrt(2)+1/2)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9576 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = # { k | n^2 ≤ k(k+1)/2 < (n+1)^2 }   (triangular numbers in [n², (n+1)²)) */
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
    seq_t n_seq = (seq_t)n;

    /* ---- lower bound: minimal k with k(k+1)/2 ≥ n² ---- */
    seq_t d_lower = (seq_t)1 + (seq_t)8 * n_seq * n_seq;
    seq_t sqrtL   = floor_sqrt(d_lower);
    seq_t k       = (sqrtL - (seq_t)1) / (seq_t)2;
    seq_t two_n_sq = (seq_t)2 * n_seq * n_seq;
    if (k * (k + (seq_t)1) < two_n_sq) {
        k += (seq_t)1;
    }

    /* ---- upper bound: maximal k with k(k+1)/2 < (n+1)² ---- */
    seq_t np1 = n_seq + (seq_t)1;
    seq_t d_upper = (seq_t)1 + (seq_t)8 * np1 * np1;
    seq_t sqrtU   = floor_sqrt(d_upper);
    seq_t kmax = (sqrtU - (seq_t)1) / (seq_t)2;
    seq_t two_np1_sq = (seq_t)2 * np1 * np1;
    if (kmax * (kmax + (seq_t)1) >= two_np1_sq) {
        kmax -= (seq_t)1;
    }

    return kmax - k + (seq_t)1;
}
