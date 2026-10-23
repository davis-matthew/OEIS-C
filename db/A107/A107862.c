/*
 * A107862  Triangle, read by rows, where T(n,k) = C(n*(n-1)/2 - k*(k-1)/2 +
 *          n-k, n-k).
 * https://oeis.org/A107862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = binomial((n-k)*(n+k+1)/2, n-k). Sequence a(n) is the
 *            triangle read by rows, flattened into a single sequence
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 135, u128 n <= 350,
 *            big any n
 * Helpers:   binom, row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/row_of_index.h"

seq_t a(idx_t n)
{
    /* Find row r and column c corresponding to index n.
       row_of_index returns r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = row_of_index(n);
    idx_t c = n - r * (r + 1) / 2;

    /* Compute binomial argument: (r-c)*(r+c+1)/2 choose (r-c) */
    idx_t k   = r - c;               /* equals n-k from formula */
    idx_t N   = (k * (r + c + 1)) / 2;

    return binom(N, k);
}
