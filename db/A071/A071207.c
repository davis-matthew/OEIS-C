/*
 * A071207  Triangular array T(n,k) read by rows, giving number of rooted
 *          trees on the vertex set {1..n+1} where k children of the root have
 *          a label smaller than the label of the root.
 * https://oeis.org/A071207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r,k) * r^(r-k) where r =
 *            floor((sqrt(8*n+1)-1)/2) k = n - r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of this row   */
    idx_t k     = n - start;         /* column index within the row */

    seq_t pow_val;
    if (r == 0)                      /* 0^0 is defined as 1 */
        pow_val = 1;
    else
        pow_val = big_pow((seq_t)r, (seq_t)(r - k));

    seq_t bin = binomial(r, k);
    return bin * pow_val;
}
