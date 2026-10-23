/*
 * A040700  Continued fraction for sqrt(727).
 * https://oeis.org/A040700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 26 if n == 0; otherwise if n is odd then 1; otherwise if
 *            n mod 4 == 2 then 25; else 52
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)26;
    } else if (n & 1) {
        return (seq_t)1;
    } else if ((n & 3) == 2) {
        return (seq_t)25;
    } else {
        return (seq_t)52;
    }
}
