/*
 * A066405  From expansion of Belyi function for octahedron.
 * https://oeis.org/A066405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 256*(-1)^n*(8*n^3 - 24*n^2 + 25*n - 9)/3 for n>2 Initial
 *            terms: a(0)=1, a(1)=-46, a(2)=769
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 136, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)(-46);
    if (n == 2) return (seq_t)769;

    seq_t nn = (seq_t)n;
    // Compute the cubic polynomial: 8*n^3 - 24*n^2 + 25*n - 9
    seq_t poly = (seq_t)8 * nn * nn * nn
               - (seq_t)24 * nn * nn
               + (seq_t)25 * nn
               - (seq_t)9;

    // The polynomial is always divisible by 3 for integer n
    seq_t res = (poly * (seq_t)256) / (seq_t)3;

    // Apply the alternating sign
    if (n & 1) res = -res;

    return res;
}
