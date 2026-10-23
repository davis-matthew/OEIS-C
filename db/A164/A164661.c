/*
 * A164661  Denominators of row sums of triangle of rationals A164658/A164659.
 *          Definite integral of Chebyshev polynomials of the first kind:
 *          Integral_{x=0..1} T(n,x).
 * https://oeis.org/A164661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(0) = 1; For even n > 0 : a(n) = n^2 - 1; For odd n : a(n) = n
 *            + 1 if n mod 4 == 1, a(n) = n - 1 if n mod 4 == 3
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & 1) == 0) {            /* even n */
        seq_t nn = (seq_t)n;
        seq_t val = nn * nn;
        val -= (seq_t)1;
        return val;
    } else {                        /* odd n */
        if ((n & 3) == 1)          /* n mod 4 == 1 */
            return (seq_t)n + (seq_t)1;
        else                       /* n mod 4 == 3 */
            return (seq_t)n - (seq_t)1;
    }
}
