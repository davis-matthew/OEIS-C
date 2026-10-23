/*
 * A038334  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*8^j.
 * https://oeis.org/A038334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 12^(i-j) * 8^j where (i,j) are obtained
 *            from the row‑major enumeration of the triangle: i = smallest
 *            non‑negative integer with (i+1)(i+2)/2 > n j = n - i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 37, u64 n <= 138, u128 n <= 502,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* find row i and column j */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        i++;
    }
    idx_t row = i;
    idx_t offset = row * (row + 1) / 2;
    idx_t j = n - offset;

    /* binomial coefficient */
    seq_t bin = binomial(row, j);

    /* powers of 12 and 8 */
    seq_t pow12 = 1;
    idx_t exp12 = row - j;
    for (idx_t k = 0; k < exp12; ++k) {
        pow12 *= (seq_t)12;
    }

    seq_t pow8 = 1;
    idx_t exp8 = j;
    for (idx_t k = 0; k < exp8; ++k) {
        pow8 *= (seq_t)8;
    }

    return bin * pow12 * pow8;
}
