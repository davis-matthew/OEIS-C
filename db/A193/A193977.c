/*
 * A193977  Triangular array: the fission of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=x*p(n-1,x)+n+1 with p(0,x)=1, and q(n,x)=sum{(k+1)*x^k ;
 *          0<=k<=n}.
 * https://oeis.org/A193977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((r - k + 1) * (k + 1) * (2*r + 4 - k)) / 2, where r =
 *            floor((sqrt(8*n+1) - 1)/2) is the row index, and k = n -
 *            r*(r+1)/2 is the column index
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the row r for index n */
    long double val = sqrtl((long double)n * 8.0L + 1.0L);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one due to floating point rounding */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k */
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* Compute the term using seq_t arithmetic */
    seq_t A = (seq_t)(r - k + 1);
    seq_t B = (seq_t)(k + 1);
    seq_t C = (seq_t)(2 * r + 4 - k);

    seq_t prod = A * B * C;
    /* The product is even; divide last to avoid overflow */
    prod /= 2;

    return prod;
}
