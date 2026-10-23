/*
 * A038269  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*3^j.
 * https://oeis.org/A038269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*7^(i-j)*3^j where (i,j) is obtained * from
 *            the row-major flattening of the triangle
 *            T(i,j)=C(i,j)*7^(i-j)*3^j. * For a given index n, i =
 *            floor((sqrt(8n+1)-1)/2) is the row number, * and j = n -
 *            i*(i+1)/2 is the column in that row
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    double d = sqrt((double)8.0 * n + 1.0);
    uint64_t i = (uint64_t)((d - 1.0) / 2.0);
    /* Correct possible rounding errors */
    while ((i + 1) * (i + 2) / 2 <= n) ++i;
    while (i * (i + 1) / 2 > n) --i;

    uint64_t start = i * (i + 1) / 2;
    uint64_t j = n - start;

    /* Compute binomial coefficient C(i,j) multiplicatively */
    seq_t binom = 1;
    for (uint64_t k = 1; k <= j; ++k) {
        binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
    }

    /* Compute powers of 7 and 3 */
    seq_t pow7 = 1;
    for (uint64_t k = 0; k < i - j; ++k) pow7 *= (seq_t)7;

    seq_t pow3 = 1;
    for (uint64_t k = 0; k < j; ++k) pow3 *= (seq_t)3;

    return binom * pow7 * pow3;
}
