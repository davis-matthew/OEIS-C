/*
 * A040195  Continued fraction for sqrt(210).
 * https://oeis.org/A040195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; a(n)=2 if n%2=1, 28 if n%2=0 (n>=1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)14;
    return (n & 1) ? (seq_t)2 : (seq_t)28;
}
