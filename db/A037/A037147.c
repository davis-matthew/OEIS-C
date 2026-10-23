/*
 * A037147  Denominators of Fourier coefficients of Eisenstein series of
 *          degree 2 and weight 10 when evaluated at Gram(A_2)*z.
 * https://oeis.org/A037147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 1 if 0 <= n <= 2, otherwise 43867
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    return (seq_t)43867;
}
