/*
 * A024224  a(n) = floor((4th elementary symmetric function of S(n))/(3rd
 *          elementary symmetric function of S(n))), where S(n) = {first n+3
 *          positive integers congruent to 1 mod 3}.
 * https://oeis.org/A024224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n^2 + 5*n - 6)/8) (OEIS A024224)
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)3 * nn * nn + (seq_t)5 * nn - (seq_t)6) / (seq_t)8;
}
