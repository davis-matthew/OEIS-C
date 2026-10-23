/*
 * A105084  Triangle read by rows: a[n, m] = a[n - 1, m] + binomial[n, m]; n
 *          <=m.
 * https://oeis.org/A105084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from a triangular array where a[m,0] = 1, a[m,m] =
 *            2 and for 1 ≤ c < m a[m,c] = 2 + Σ_{k=c+1}^{m} binomial(k+1,
 *            c+1)
 * Range:     u8 n <= 46, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   binomial, integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is read by rows, so given n we determine row r
   and column c with r(r+1)/2 ≤ n < (r+1)(r+2)/2,
   then return a[r,c].                                    */

#include "../utils/binomial.h"
#include "../utils/integer_sqrt.h"

seq_t a(idx_t n)
{
    /* find row r: r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (integer_sqrt(8*n + 1) - 1) / 2;

    /* column index within the row */
    idx_t c = n - r*(r+1)/2;

    if (c == 0) /* first element of every row */
        return (seq_t)1;

    if (c == r) /* last element of every row */
        return (seq_t)2;

    /* intermediate elements */
    seq_t value = (seq_t)2;
    for (idx_t k = c+1; k <= r; ++k)
        value += binomial(k + 1, c + 1);

    return value;
}
