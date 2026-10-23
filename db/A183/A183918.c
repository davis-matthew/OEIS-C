/*
 * A183918  Characteristic sequence for cos(2*Pi/n) being rational.
 * https://oeis.org/A183918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if cos(2*PI/n) is rational, else 0. Rational
 *            cos(2*PI/n) occurs only for n = 1, 2, 3, 4, 6
 * Range:     u8 n <= 127, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 1 for n in {1,2,3,4,6}, otherwise 0. */
    if (n == 1 || n == 2 || n == 3 || n == 4 || n == 6)
        return (seq_t)1;
    return (seq_t)0;
}
