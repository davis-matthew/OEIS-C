/*
 * A005530  Number of Boolean functions of n variables from Post class
 *          F(8,inf); number of degenerate Boolean functions of n variables.
 * https://oeis.org/A005530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{j=1..n} (-1)^(j+1)*binomial(n,j)*2^(2^(n-j))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t p = 2; /* 2^(2^(n-j)), starting at j = n */
    idx_t j;
    for (j = n; j >= 1; j--) {
        seq_t term = binom(n, j) * p;
        if (j % 2 == 1)
            result += term;
        else
            result -= term;
        p = p * p;
    }
    return result;
}
