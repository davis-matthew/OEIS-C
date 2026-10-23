/*
 * A149713  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 1), (-1, 1,
 *          1), (0, 0, -1), (1, -1, 1), (1, 1, 1)}.
 * https://oeis.org/A149713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   Let d be the number of down steps (z–) (0 ≤ d ≤ ⌊n/2⌋) and u = n
 *            – d the number of up steps (z+). For a fixed (u,d) the number of
 *            admissible z‑sequences is the ballot number B(u,d) = (u - d +
 *            1)/(u + 1) * C(u + d , d). The x‑ and y‑coordinates change only
 *            during up steps. Each up step independent...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over number of down steps d */
    for (idx_t d = 0; d <= n / 2; ++d)
    {
        idx_t u = n - d;            /* number of up steps        */
        /* Ballot number B(u,d) */
        seq_t B = binom(u + d, d)
                  * (seq_t)(u - d + 1)
                  / (seq_t)(u + 1);

        /* D(u) = C(u, floor(u/2)) */
        idx_t k = u >> 1;           /* floor(u/2)                */
        seq_t Du = binom(u, k);

        /* term = B * Du^2 */
        seq_t term = B * Du * Du;
        sum += term;
    }
    return sum;
}
