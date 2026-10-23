/*
 * A040120  Continued fraction for sqrt(132).
 * https://oeis.org/A040120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 11; for n≥1, a(n) = (n%2==1 ? 2 : 22)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;
    return (n & 1) ? (seq_t)2 : (seq_t)22;
}
