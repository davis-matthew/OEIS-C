/*
 * A149259  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          0), (1, 1, -1), (1, 1, 1)}.
 * https://oeis.org/A149259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = ∑_{d=0}^{⌊n/2⌋} binom(n - d, ⌊(n - d)/2⌋) * 2^d * ((n -
 *            2d + 1)/(n - d + 1)) * binom(n, d) where binom(a, b) is the
 *            binomial coefficient. The division is always integral. This
 *            formula counts, for each possible number d of “down” steps in
 *            the auxiliary one‑dimensional walk (corresponding to...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_d = n / 2;
    seq_t pow2d = 1;                     /* 2^d, d starts at 0 */

    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t u = n - d;                 /* number of “up” steps (+1,+1) */
        idx_t k = u / 2;                 /* floor(u/2) for the z‑assignment */

        /* term1 = binom(u, floor(u/2)) */
        seq_t term1 = binom(u, k);

        /* term2 = binom(n, d) */
        seq_t term2 = binom(n, d);

        /* term3 = ((n - 2d + 1) / (n - d + 1)) * binom(n, d) */
        seq_t num = term2 * (seq_t)(n - 2 * d + 1);
        seq_t denom = (seq_t)(n - d + 1);
        seq_t term3 = num / denom;       /* exact division */

        /* contribution for this d */
        seq_t contrib = term1 * pow2d * term3;

        total += contrib;

        /* update 2^d for next iteration: multiply by 2 */
        pow2d *= 2;
    }
    return total;
}
