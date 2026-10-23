/*
 * A035287  Number of ways to place a non-attacking white and black rook on n
 *          X n chessboard.
 * https://oeis.org/A035287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n-1)^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    // Convert index to seq_t first to avoid mixing types
    seq_t nn = (seq_t)n;
    seq_t nm1 = nn - (seq_t)1;

    // Compute n^2 and (n-1)^2
    seq_t n_sq = nn * nn;
    seq_t nm1_sq = nm1 * nm1;

    // Result is the product of the two squares
    return n_sq * nm1_sq;
}
