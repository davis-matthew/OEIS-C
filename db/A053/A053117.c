/*
 * A053117  Triangle read by rows of coefficients of Chebyshev's U(n,x)
 *          polynomials (exponents in increasing order).
 * https://oeis.org/A053117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign, tabl, look, changed
 * Formula:   For the flattened sequence index n write r =
 *            floor((sqrt(8*n+1)-1)/2) (row of the triangle), k = n –
 *            r*(r+1)/2 (column in that row). If r < k or (r + k) is odd then
 *            a(n) = 0. Otherwise set t = (r + k)/2 and compute a(n) =
 *            (-1)^(t+k) * 2^k * binomial(t, k)
 * Range:     i8 n <= 32, i16 n <= 112, i32 n <= 394, i64 n <= 1411,
 *            i128 n <= 5150, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute row r and column k from the flattened index n */
    double s   = sqrt((double)8.0 * (double)n + 1.0);
    idx_t   r  = (idx_t)((s - 1.0) * 0.5);          /* initial estimate */

    /* adjust r to satisfy r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while ( ((__uint128_t)(r + 1) * (r + 2) / 2) <= n )
        r++;
    while ( ((__uint128_t)r * (r + 1) / 2) > n )
        r--;

    idx_t tri = (idx_t)((__uint128_t)r * (r + 1) / 2);   /* T(r) */
    idx_t k   = n - tri;                                /* column index */

    /* check parity and bounds */
    if (r < k || ((r + k) & 1))
        return (seq_t)0;

    idx_t t   = (r + k) / 2;
    seq_t bin   = binom(t, k);          /* binomial coefficient */
    seq_t powv  = pow2(k);              /* 2^k */
    seq_t res   = bin * powv;           /* absolute value */

    /* apply sign: (-1)^(t + k) */
    if (((t + k) & 1) != 0)
        res = -res;

    return res;
}
