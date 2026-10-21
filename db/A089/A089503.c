/*
 * A089503  Triangle of numbers used for basis change between certain falling
 *          factorials.
 * https://oeis.org/A089503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the (row r, column m) entry of T(r,m) = binomial(r-1,
 *            m-1) * binomial(2r, m-1) * (m-1)! , where r is the smallest
 *            integer with r(r+1)/2 ≥ n and m = n – r(r-1)/2. a(1)=1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; callers should use n ≥ 1 */
        return 0;

    /* Find row r: smallest integer with r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        r++;

    /* Number of terms before row r. */
    idx_t prev = (r - 1) * r / 2;

    /* Column index within row r (1 ≤ m ≤ r). */
    idx_t m = n - prev;

    /* Compute the triangle entry T(r,m). */
    seq_t res = binomial(r - 1, m - 1);                 /* binomial(r-1, m-1) */
    res *= binomial((idx_t)(2 * r), m - 1);             /* binomial(2r, m-1) */
    res *= factorial(m - 1);                           /* (m-1)! */

    return res;
}
