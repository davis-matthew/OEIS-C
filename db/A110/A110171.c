/*
 * A110171  Triangle read by rows: T(n,k) (0 <= k <= n) is the number of
 *          Delannoy paths of length n that start with exactly k (0,1) steps
 *          (or, equivalently, with exactly k (1,0) steps).
 * https://oeis.org/A110171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T( row_of_index(n), n -
 *            row_of_index(n)*(row_of_index(n)+1)/2 ) where T(n,k) =
 *            Sum_{i=0}^{n-k} binomial(n,i) * binomial(2*n-k-i-1, n-k-i)
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 104, u64 n <= 377,
 *            u128 n <= 1430, big any n
 * Helpers:   binomial, mul128, row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_of_index.h"
#include "../utils/binomial.h"
#include "../utils/mul128.h"

static seq_t triangle_T(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (n == 0) return (seq_t)1;           /* T(0,0) = 1 */

    seq_t total = (seq_t)0;
    idx_t i_max = n - k;                    /* maximum i with non‑negative binomial */
    for (idx_t i = 0; i <= i_max; ++i)
    {
        seq_t c1 = binomial(n, i);

        idx_t N2 = 2*n - k - i - 1;
        idx_t K2 = n - k - i;
        seq_t c2 = binomial(N2, K2);

        seq_t prod = mul128(c1, c2);       /* exact product if seq_t supports it   */
        total += prod;                     /* addition may overflow only if the
                                              final value does not fit in seq_t   */
    }
    return total;
}

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = row_of_index(n);

    /* column k in that row */
    idx_t k = n - r * (r + 1) / 2;

    return triangle_T(r, k);
}
