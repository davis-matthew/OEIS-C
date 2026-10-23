/*
 * A122553  a(0)=1, a(n)=3 for n > 0.
 * https://oeis.org/A122553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, cons
 * Formula:   a(0) = 1; a(n) = 3 for n > 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)1 : (seq_t)3;
}
