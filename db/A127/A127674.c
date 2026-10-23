/*
 * A127674  Coefficient table for Chebyshev polynomials T(2*n,x) (increasing
 *          even powers x, without zeros).
 * https://oeis.org/A127674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = coefficient of the even power x^(2m) in the Chebyshev
 *            polynomial T(2*r, x) enumerated row‑wise: a(r,m) = 0 (m>r)
 *            (-1)^(r-m) * 2^(2*m-1) * binomial(r+m, 2*m) * (2*r)/(r+m) with
 *            a(r,0) = (-1)^r. The global index n is mapped to row r and
 *            offset m by the usual triangular numbering: r is the un...
 * Range:     i8 n <= 7, i16 n <= 32, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* --- determine row r and offset m from global index n --- */
    /* compute the integer r satisfying  r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    /* t = 8*n + 1 ; r = floor( (sqrt(t)-1)/2 ) */
    seq_t t = (seq_t)8 * (seq_t)n + 1;
    seq_t sq = floor_sqrt(t);          /* floor sqrt of 8*n+1 */
    idx_t r = (idx_t)((sq - 1) / 2);   /* row number */

    /* adjust in case of off‑by‑one due to rounding */
    idx_t prev_cnt = r * (r + 1) / 2;   /* total elements up to previous row */
    if (prev_cnt > n)
    {
        r--;
        prev_cnt = r * (r + 1) / 2;
    }

    idx_t m = n - prev_cnt;            /* position within the row (0‑based) */

    /* --- compute coefficient a(r,m) --- */
    if (m == 0)
    {
        /* first coefficient of each row */
        return (seq_t)((r & 1) ? -1 : 1);
    }

    /* sign factor (-1)^(r-m) */
    seq_t sign = ((r - m) & 1) ? (seq_t)-1 : (seq_t)1;

    /* 2^(2*m-1) */
    seq_t pow2_val = pow2(2 * m - 1);

    /* binomial(r+m, 2*m) */
    seq_t binom_val = binomial(r + m, 2 * m);

    /* Compute numerator: pow2_val * binom_val * (2*r) */
    __int128 temp = ( __int128 )pow2_val * ( __int128 )binom_val;
    temp *= ( __int128 )( 2 * ( seq_t )r );

    /* Divide by (r+m) (the division is exact) */
    temp /= ( __int128 )( r + m );

    /* Apply sign */
    temp *= ( __int128 )sign;

    return (seq_t)temp;
}
