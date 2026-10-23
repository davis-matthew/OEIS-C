/*
 * A117317  Triangle related to partitions of n.
 * https://oeis.org/A117317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = max integer with r*(r+1)/2 <= n * k =
 *            n - r*(r+1)/2 * and * T(r,k) = sum_{j=0..r-k} C(r+j,k) *
 *            C(r-k,j) * Here C is the ordinary binomial coefficient. * The
 *            row with index r starts at position r*(r+1)/2 in the *
 *            concatenated list of the triangle’s rows
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 215, u64 n <= 754,
 *            u128 n <= 2874, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Locate the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    seq_t sum = 0;
    idx_t max_j = r - k;            /* j runs 0 .. r-k */
    for (idx_t j = 0; j <= max_j; ++j)
    {
        seq_t term1 = binom(r + j, k);
        seq_t term2 = binom(r - k, j);
        sum += term1 * term2;
    }
    return sum;
}
