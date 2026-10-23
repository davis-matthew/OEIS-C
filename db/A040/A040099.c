/*
 * A040099  Continued fraction for sqrt(110).
 * https://oeis.org/A040099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 10; for n >= 1, a(n) = 2 if n is odd, 20 if n is even
 *            (Continued fraction for sqrt(110))
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)10;
    }
    /* n >= 1 */
    if ((n & (idx_t)1) == (idx_t)1) {    /* odd */
        return (seq_t)2;
    } else {                            /* even */
        return (seq_t)20;
    }
}
