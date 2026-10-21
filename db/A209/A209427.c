/*
 * A209427  T(n,k) = binomial(n,k)^n.
 * https://oeis.org/A209427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,c)^r, where r is the smallest integer such
 *            that r*(r+1)/2 <= n, and c = n - r*(r+1)/2 (i.e. rows of the
 *            triangle are flattened in the natural order)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 57, u128 n <= 95,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find row r and column c such that n = r*(r+1)/2 + c, with 0 <= c <= r. */
    idx_t r = 0;
    idx_t sum = 0; /* total number of terms before row r (i.e., S(r-1)). */
    while (n >= sum + (r + 1)) {
        sum += r + 1;
        ++r;
    }
    idx_t c = n - sum;

    /* Compute binomial coefficient C(r,c) and raise it to the power r. */
    seq_t bin = binomial(r, c);
    return big_pow(bin, (seq_t)r);
}
