/*
 * A076256  Coefficients of the polynomials in the numerator of 1/(1+x^2) and
 *          its successive derivatives, starting with the constant term.
 * https://oeis.org/A076256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) where r is the unique integer with r*(r+1)/2 <= n
 *            < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. T(r,k) = 0 if r+k is odd
 *            T(r,k) = (-1)^((r+k)/2) * r! * binomial(r+1, k) otherwise
 * Range:     i8 n <= 11, i16 n <= 28, i32 n <= 68, i64 n <= 159,
 *            i128 n <= 445, big any n
 * Helpers:   binomial_u64, factorial_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial_u64.h"
#include "../utils/factorial_u64.h"

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = (idx_t)((sqrtl((long double)n * 8.0L + 1.0L) - 1.0L) / 2.0L);

    /* Correct possible inaccuracy of the sqrt by adjusting. */
    __int128 lower = (__int128)r * (r + 1) / 2;
    while (lower > n)
    {
        --r;
        lower = (__int128)r * (r + 1) / 2;
    }
    __int128 upper = (__int128)(r + 1) * (r + 2) / 2;
    while (upper <= n)
    {
        ++r;
        upper = (__int128)(r + 1) * (r + 2) / 2;
    }

    idx_t k = n - (idx_t)lower;  /* k is the index within row r */

    /* Parity check */
    if ( ((r + k) & 1) )
        return (seq_t)0;

    /* Sign = (-1)^((r+k)/2) */
    int sign = (((r + k) / 2) & 1) ? -1 : 1;

    /* Compute factorial(r) and binomial(r+1, k) */
    seq_t fact = factorial_u64(r);
    seq_t comb = binomial_u64(r + 1, k);

    seq_t val = fact * comb;
    return sign * val;
}
