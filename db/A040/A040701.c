/*
 * A040701  Continued fraction for sqrt(728).
 * https://oeis.org/A040701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=26; for n>0, a(n)=1 if n odd, otherwise 52
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)26;
    return (n & 1) ? (seq_t)1 : (seq_t)52;
}
