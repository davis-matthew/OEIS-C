/*
 * A156889  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1} ( (k+1)^6
 *          -(k+1)^5 -(k+1)^4 +(k+1)^2 )^i ) with T(n, 0) = n!, read by
 *          antidiagonals.
 * https://oeis.org/A156889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value on the antidiagonal of the square array T(n,k)
 *            defined by T(n,k) = Π_{j=1..n} Σ_{i=0..j-1} (
 *            ((k+1)^6-(k+1)^5-(k+1)^4+(k+1)^2) )^i with T(n,0) = n! . The
 *            sequence is read by antidiagonals: a(0)=T(0,0), a(1)=T(0,1),
 *            a(2)=T(1,0),
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 31, u128 n <= 41,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t m)
{
    /* special case m=0: T(0,0)=1 */
    if (m == 0u) return (seq_t)1;

    /* find antidiagonal index s such that
        T_{s} = (s+1)(s+2)/2 > m >= s(s+1)/2
    */
    idx_t s = 0;
    while ( ((s + 1u) * (s + 2u)) / 2u <= m ) ++s;

    idx_t offset = m - s * (s + 1u) / 2u;   /* position inside the s-th antidiagonal */
    idx_t row = offset;                    /* n */
    idx_t col = s - offset;                /* k */

    /* T(n,0) = n! */
    if (col == 0u) return factorial(row);

    /* compute T(row,col) using the product formula */
    seq_t kplus1 = (seq_t)col + 1u;         /* k+1 */
    seq_t x2 = kplus1 * kplus1;             /* (k+1)^2 */
    seq_t x3 = x2 * kplus1;                 /* (k+1)^3 */
    seq_t x4 = x3 * kplus1;                 /* (k+1)^4 */
    seq_t x5 = x4 * kplus1;                 /* (k+1)^5 */
    seq_t x6 = x5 * kplus1;                 /* (k+1)^6 */
    seq_t f = x6 - x5 - x4 + x2;            /* f(k) = (k+1)^6 - (k+1)^5 - (k+1)^4 + (k+1)^2 */
    seq_t denom = f - 1u;                   /* f-1, used as divisor */

    /* handle degenerate case f==1 (impossible for k>=1) */
    if (denom == 0u) return factorial(row);

    seq_t pow = f;
    seq_t result = 1u;

    for (idx_t j = 1u; j <= row; ++j)
    {
        seq_t term = (pow - 1u) / denom;    /* (f^j - 1)/(f-1) = 1 + f + ... + f^{j-1} */
        result *= term;
        pow *= f;                           /* advance f^j */
    }

    return result;
}
