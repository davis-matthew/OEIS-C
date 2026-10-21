/*
 * A059978  a(n) = binomial(n+2,n)^6.
 * https://oeis.org/A059978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2, n)^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 55, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the binomial helper from the utilities. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* binomial(n+2, n) is (n+2 choose 2) = (n+1)(n+2)/2 */
    seq_t bin = binom(n + 2, n);
    seq_t res = bin;
    for (idx_t i = 1; i < 6; ++i)
        res *= bin;
    return res;
}
