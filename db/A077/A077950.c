/*
 * A077950  Expansion of 1/(1-x+2*x^3).
 * https://oeis.org/A077950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-3), with a(0)=1, a(1)=1, a(2)=1
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2) {
        return (seq_t)1;
    }
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    seq_t a3;
    for (idx_t i = 3; i <= n; ++i) {
        a3 = a2 - ((seq_t)2 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
