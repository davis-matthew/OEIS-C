/*
 * A040181  Continued fraction for sqrt(195).
 * https://oeis.org/A040181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 13 if n == 0; otherwise a(n) = 26 if n has a factor 2 (n
 *            even), else 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)13;
    }
    if ((n & 1) == 0) {
        return (seq_t)26;
    }
    return (seq_t)1;
}
