/*
 * A087056  Difference between 2 * n^2 and the next smaller square number.
 * https://oeis.org/A087056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n^2 - floor(n * sqrt(2))^2
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 2*n^2 */
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t two_n_sq = nn + nn;           /* 2*n^2 */

    /* k = floor(n * sqrt(2)) = floor(sqrt(2*n^2)) */
    seq_t k = floor_sqrt(two_n_sq);

    /* a(n) = 2*n^2 - k^2 */
    seq_t kk = k * k;
    return two_n_sq - kk;
}
