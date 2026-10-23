/*
 * A040157  Continued fraction for sqrt(171).
 * https://oeis.org/A040157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=13, a(1)=13, for n≥2 a(n)=13 if n is odd, a(n)=26 if n is
 *            even
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 13;
    }
    /* n%2==0 means n is even */
    if ((n & 1) == 0) {
        return 26;
    } else {
        return 13;
    }
}
