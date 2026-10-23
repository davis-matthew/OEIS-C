/*
 * A038278  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*12^j.
 * https://oeis.org/A038278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 7^(i-j) * 12^j, * where (i,j) are the row
 *            and column indices in the * triangle such that n = i*(i+1)/2 + j
 *            (row‑major order)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }

    /* Column j is the offset within the row. */
    idx_t j = n - i * (i + 1) / 2;

    /* Compute the triangle entry. */
    seq_t val = binomial(i, j);

    /* Power of 7: 7^(i-j). */
    seq_t pow7 = 1;
    for (idx_t k = 0; k < i - j; ++k) {
        pow7 *= (seq_t)7;
    }

    /* Power of 12: 12^j. */
    seq_t pow12 = 1;
    for (idx_t k = 0; k < j; ++k) {
        pow12 *= (seq_t)12;
    }

    val *= pow7;
    val *= pow12;

    return val;
}
