/*
 * A053120  Triangle of coefficients of Chebyshev's T(n,x) polynomials (powers
 *          of x in increasing order).
 * https://oeis.org/A053120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, nice, easy, changed
 * Formula:   a(n) = T(r,c) where r and c are obtained from the linear index n
 *            by row = largest r with r*(r+1)/2 <= n c = n - r*(r+1)/2 T(r,c)
 *            are the Chebyshev T(r,x) coefficients in increasing powers of x.
 *            T(r,c) is defined by: T(r,c) = 0 if r < c or r+c is odd T(r,0) =
 *            (-1)^(r/2) if r even T(r,c) = (-1)^( (...
 * Range:     i8 n <= 39, i16 n <= 114, i32 n <= 396, i64 n <= 1463,
 *            i128 n <= 5150, big any n
 * Validated: standard, pass, 94 terms through n = 5111 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t chebyshev_T(idx_t n, idx_t m)
{
    /* parity test: if n+m is odd, coefficient is zero */
    if (n < m) return 0;
    if ((n + m) & 1) return 0;

    /* special case m == 0 */
    if (m == 0) {
        if ((n & 1) == 1) return 0;           /* odd n -> zero */
        /* even n: (-1)^(n/2) */
        return (( (n/2) & 1 ) ? (seq_t)-1 : (seq_t)1);
    }

    /* general case m >= 1 */
    idx_t half = (n + m) / 2;                /* (n+m)/2   */
    idx_t k    = half - 1;                   /* numerator upper index for binomial */
    idx_t r    = m - 1;                       /* binomial lower index */

    /* compute binomial C(k, r) multiplicatively */
    __int128 binom = 1;
    for (idx_t i = 1; i <= r; ++i) {
        binom = binom * (k - r + i) / i;
    }

    /* multiply by n and divide by m */
    __int128 num = binom * (__int128)n;
    __int128 t   = num / (__int128)m;          /* integer division is exact */

    /* multiply by 2^(m-1) */
    __int128 pow2 = 1;
    for (idx_t i = 1; i < m; ++i) pow2 *= 2;

    t *= pow2;

    /* apply sign: (-1)^(half + m) */
    if ( ((half + m) & 1) != 0 ) t = -t;

    return (seq_t)t;
}

seq_t a(idx_t n)
{
    /* determine row r and column c from the linear index n */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n ) r++;
    idx_t c = n - r * (r + 1) / 2;

    return chebyshev_T(r, c);
}
