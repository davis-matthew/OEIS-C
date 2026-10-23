/*
 * A038262  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*8^j.
 * https://oeis.org/A038262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binom(i, j) * 6^(i-j) * 8^j where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2. The sequence is
 *            the row‑major flattening of the triangle T(i, j) = binom(i, j) *
 *            6^(i-j) * 8^j
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t k = 0; k < exp; ++k)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row i such that the prefix sum of row lengths
       up to i-1 is <= n < prefix sum up to i. */
    idx_t i = 0;
    idx_t tri = 0;               /* number of terms before row i */
    while (i + 1 <= n - tri)     /* safe: n - tri is defined because tri <= n */
    {
        tri += i + 1;
        ++i;
    }
    idx_t j = n - tri;           /* column within the row */

    seq_t bin = binom(i, j);
    seq_t pow6   = pow_seq((seq_t)6, i - j);
    seq_t pow8   = pow_seq((seq_t)8, j);

    return bin * pow6 * pow8;
}
