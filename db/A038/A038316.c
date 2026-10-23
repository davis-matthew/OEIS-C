/*
 * A038316  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*2^j.
 * https://oeis.org/A038316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 2^j where i is the row
 *            containing index n in the triangular layout and j = n −
 *            i*(i+1)/2 is the column position in that row
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, seq_pow_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow_ll.h"

seq_t a(idx_t n)
{
    /* Find the row i (named r here) such that
       T(r-1) <= n < T(r), where T(r) = (r+1)(r+2)/2. */
    idx_t r = 0;
    while (n >= ((r + 1) * (r + 2)) / 2)
        ++r;

    /* Compute column index j = n - previous total. */
    idx_t prev_total = r * (r + 1) / 2;
    idx_t j = n - prev_total;

    /* Compute the binomial coefficient and the powers. */
    seq_t bin = binomial(r, j);
    seq_t pow11 = seq_pow_ll((seq_t)11, r - j);
    seq_t pow2  = seq_pow_ll((seq_t)2,  j);

    /* Combine the factors. */
    seq_t res = bin * pow11;
    res *= pow2;
    return res;
}
