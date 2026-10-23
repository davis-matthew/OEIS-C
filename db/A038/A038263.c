/*
 * A038263  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*9^j.
 * https://oeis.org/A038263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*6^(i-j)*9^j where i and j satisfy * n =
 *            i*(i+1)/2 + j, 0 <= j <= i
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    idx_t tri = 0;
    while (tri <= n) {
        i++;
        tri = i * (i + 1) / 2;
    }
    i--;                     /* now i*(i+1)/2 <= n */
    tri = i * (i + 1) / 2;
    idx_t j = n - tri;       /* 0 <= j <= i */

    /* Compute binomial(i,j) multiplicatively */
    seq_t bin = 1;
    for (idx_t k = 1; k <= j; ++k) {
        bin *= (seq_t)(i - k + 1);
        bin /= (seq_t)k;
    }

    /* Multiply by powers of 6 and 9 */
    seq_t val = bin;
    for (idx_t k = 0; k < i - j; ++k) val *= (seq_t)6;
    for (idx_t k = 0; k < j; ++k)   val *= (seq_t)9;

    return val;
}
