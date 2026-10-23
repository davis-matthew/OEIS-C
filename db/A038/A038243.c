/*
 * A038243  Triangle whose (i,j)-th entry is 5^(i-j)*binomial(i,j).
 * https://oeis.org/A038243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 5^(i-j) * binomial(i,j) where i = floor((sqrt(8 n + 1) –
 *            1) / 2) and j = n – i(i+1)/2. This gives the n‑th term of the
 *            flattened triangle of (5+x)^i
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 104, u64 n <= 352,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row i */
    idx_t i = (idx_t)((sqrt((long double)8 * n + 1) - 1) / 2);
    /* first index of row i */
    idx_t start = (idx_t)(((seq_t)i * (i + 1)) / 2);
    idx_t j = n - start;              /* column j in row i */
    idx_t exp = i - j;                 /* power of 5 */

    /* binomial(i,j) via multiplicative formula */
    idx_t k = j;
    if (k > i - k)
        k = i - k;                     /* use symmetry */
    seq_t bin = 1;
    for (idx_t s = 1; s <= k; ++s) {
        bin = bin * (i - k + s);
        bin = bin / s;
    }

    /* 5^exp */
    seq_t pow5 = 1;
    for (idx_t e = 0; e < exp; ++e)
        pow5 *= 5;

    return bin * pow5;
}
