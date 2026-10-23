/*
 * A157481  Number of primes between n^3-n^2 and (n+1)^3-(n+1)^2.
 * https://oeis.org/A157481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 * a(n) = π(n³ - n²) - π((n-1)³ - (n-1)²) for
 *            n ≥ 2 * Here π(x) is the prime counting function
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    // upper = n^3 - n^2 = n^2 * (n-1)
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t upper = n_sq * (seq_t)(n - 1);

    // lower = (n-1)^3 - (n-1)^2 = (n-1)^2 * (n-2)
    idx_t nm1   = n - 1;
    seq_t nm1_sq = (seq_t)nm1 * (seq_t)nm1;
    seq_t lower  = nm1_sq * (seq_t)(nm1 - 1);

    seq_t pi_upper = compute_pi((idx_t)upper);
    seq_t pi_lower = compute_pi((idx_t)lower);

    return pi_upper - pi_lower;
}
