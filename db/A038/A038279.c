/*
 * A038279  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*1^j.
 * https://oeis.org/A038279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, k) * 8^(r - k) where r and k are determined
 *            by r = floor( (sqrt(8*n+1) - 1) / 2 ) k = n - r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 65, u64 n <= 231, u128 n <= 903,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates the rows of the triangle
   T(i,j) = binomial(i,j) * 8^(i-j) in row-major order. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in that row.
       The cumulative number of elements up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }
    /* Column index within the row. */
    idx_t k = n - cumulative;

    /* Compute the triangle entry: binomial(r, k) * 8^(r-k). */
    seq_t binom = binomial(r, k);
    /* 8^(r-k) = 2^(3*(r-k)) */
    idx_t exp = 3 * (r - k);
    seq_t pow = pow2(exp);

    return binom * pow;
}
