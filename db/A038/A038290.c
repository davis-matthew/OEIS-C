/*
 * A038290  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*12^j.
 * https://oeis.org/A038290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where T(i,j)=C(i,j)*8^(i-j)*12^j * and (i,j) are
 *            found by mapping n to the i-th antidiagonal * of the triangle: n
 *            = i*(i+1)/2 + j, 0≤j≤i
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate i: largest i with i*(i+1)/2 ≤ n */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* column index j */
    idx_t triang = i * (i + 1) / 2;
    idx_t j = n - triang;

    /* compute binomial coefficient C(i,j) multiplicatively */
    seq_t binom = 1;
    for (idx_t k = 1; k <= j; ++k) {
        binom = binom * (seq_t)(i - j + k);
        binom = binom / (seq_t)k;
    }

    /* compute 8^(i-j) */
    seq_t pow8 = 1;
    for (idx_t k = 0; k < (i - j); ++k)
        pow8 *= (seq_t)8;

    /* compute 12^j */
    seq_t pow12 = 1;
    for (idx_t k = 0; k < j; ++k)
        pow12 *= (seq_t)12;

    return binom * pow8 * pow12;
}
