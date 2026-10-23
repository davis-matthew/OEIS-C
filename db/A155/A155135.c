/*
 * A155135  Integers k such that k^3+28*k^2 is a square.
 * https://oeis.org/A155135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 4*n - 24 for n > 7 with initial values: a(1) = -28,
 *            a(2) = -27, a(3) = -24, a(4) = -19, a(5) = -12, a(6) = -3, a(7)
 *            = 0
 * Range:     i8 n <= 14, i16 n <= 183, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7) {
        switch ((uint64_t)n) {
            case 1: return (seq_t)(-28);
            case 2: return (seq_t)(-27);
            case 3: return (seq_t)(-24);
            case 4: return (seq_t)(-19);
            case 5: return (seq_t)(-12);
            case 6: return (seq_t)(-3);
            case 7: return (seq_t)(0);
        }
    }
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)4 * nn - (seq_t)24;
}
