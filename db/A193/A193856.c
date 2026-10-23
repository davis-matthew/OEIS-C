/*
 * A193856  Triangular array: the fission of (p(n,x)) by ((2x+1)^n), where
 *          p(n,x)=(x+1)^n.
 * https://oeis.org/A193856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula for A193856:
      a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) is the row number,
      k = n - r*(r+1)/2 is the column number.
      T(r,0) = 1
      T(r,k) = -T(r,k-1) + C(r+1,k) * 3^k    (for k>=1)
*/

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t col = n - r * (r + 1) / 2;
    if (col == 0)                 /* first element of every row is 1   */
        return (seq_t)1;

    seq_t prev = 1;                /* T(r,0)                                */
    seq_t pow3 = 1;                /* 3^0   */
    idx_t N    = r + 1;            /* n+1 in the binomial formula           */

    /* compute successive elements using the recurrence */
    for (idx_t j = 1; j <= col; ++j) {
        pow3 = pow3 * (seq_t)3;          /* 3^j */
        seq_t bin = binom(N, j);         /* C(r+1,j) */
        seq_t term = bin * pow3;         /* C(r+1,j) * 3^j */
        seq_t cur = term - prev;         /* T(r,j)   */
        prev = cur;
    }
    return prev;
}
