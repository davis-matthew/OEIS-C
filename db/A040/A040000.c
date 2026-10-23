/*
 * A040000  a(0)=1; a(n)=2 for n >= 1.
 * https://oeis.org/A040000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, cons, changed
 * Formula:   a(0) = 1, a(n) = 2 for n >= 1
 * Range:     u8 n <= 9891, u16 n <= 9891, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)1 : (seq_t)2;
}
