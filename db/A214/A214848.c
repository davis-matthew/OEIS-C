/*
 * A214848  First difference of A022846.
 * https://oeis.org/A214848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*sqrt(2)+1/2) - floor(n*sqrt(2)+1/2)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = # { k | n^2 ≤ k(k+1)/2 < (n+1)^2 }   (triangular numbers in [n², (n+1)²)) */
#include "../utils/floor_sqrt.h"

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
