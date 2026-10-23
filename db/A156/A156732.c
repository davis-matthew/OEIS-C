/*
 * A156732  Triangle T(n, k) = ((n-2*k)^2/(n-k+1))*binomial(n+1, k+1), read by
 *          rows.
 * https://oeis.org/A156732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = ((n-2k)^2/(n-k+1)) * binomial(n+1, k+1) Sequence a(n)
 *            is obtained by reading the triangle T row-wise, with offset 0
 * Range:     u8 n <= 30, u16 n <= 157, u32 n <= 604, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c that correspond to linear index n.
       The r-th row (r starting from 0) contains r+1 terms, so the
       number of elements before row r is r*(r+1)/2. */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + r + 1 <= n) {
        offset += r + 1;
        r++;
    }
    idx_t c = n - offset;

    /* Compute the terms of the formula in seq_t arithmetic. */
    seq_t rr = (seq_t)r;
    seq_t cc = (seq_t)c;

    seq_t delta = rr - 2 * cc;           /* (n-2k) */
    delta *= delta;                      /* (n-2k)^2 */

    seq_t bin  = binomial((idx_t)r + 1, (idx_t)c + 1);
    seq_t denom = rr - cc + 1;            /* (n-k+1) */

    /* Cancel common factors to keep intermediate size small. */
    seq_t g = gcd(denom, bin);
    bin /= g;
    denom /= g;

    g = gcd(denom, delta);
    delta /= g;
    denom /= g;

    /* Final division is exact. */
    seq_t prod = bin * delta;
    if (denom == 1)
        return prod;
    else
        return prod / denom;
}
