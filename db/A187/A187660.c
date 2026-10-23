/*
 * A187660  Triangle read by rows: T(n,k) =
 *          (-1)^(floor(3*k/2))*binomial(floor((n+k)/2),k), 0 <= k <= n.
 * https://oeis.org/A187660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = (-1)^⌊3k/2⌋ * binomial(⌊(r+k)/2⌋, k) where r = the
 *            largest integer such that r(r+1)/2 ≤ n, k = n – r(r+1)/2. (This
 *            flattens the triangle T(n,k) defined in the OEIS entry.)
 * Range:     i8 n <= 70, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    /* For n ≤ 10 000 a linear search is trivial, for larger n it
       stays O(√n) and still cheap. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k in row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Argument for the binomial coefficient. */
    idx_t m = (r + k) / 2;

    /* Compute binomial(m, k). */
    seq_t bin = binomial(m, k);

    /* Sign factor exp = floor(3k/2). */
    idx_t exp = (3 * k) / 2;

    /* Apply the sign. */
    if (exp & 1UL)
        return -bin;
    else
        return bin;
}
