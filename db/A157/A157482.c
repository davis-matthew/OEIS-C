/*
 * A157482  Number of primes between n^3-n^2-n^1 and (n+1)^3-(n+1)^2-(n+1)^1.
 * https://oeis.org/A157482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(n³ − n² − n) − π((n−1)³ − (n−1)² − (n−1))
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; for n ≤ 1 the value is 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Compute L(n) = n³ − n² − n */
    idx_t nn   = n;
    idx_t n2   = nn * nn;
    idx_t n3   = n2 * nn;
    idx_t L    = n3 - n2 - nn;
    seq_t piL  = compute_pi(L);

    /* Compute L(n‑1) only when n‑1 ≥ 2; for n‑1 = 1 the value is 0. */
    seq_t piLprev = 0;
    if (nn > 2) {
        idx_t np   = nn - 1;
        idx_t np2  = np * np;
        idx_t np3  = np2 * np;
        idx_t Lprev = np3 - np2 - np;
        piLprev = compute_pi(Lprev);
    }

    return piL - piLprev;
}
