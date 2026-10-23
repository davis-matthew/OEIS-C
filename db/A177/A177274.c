/*
 * A177274  Periodic sequence: Repeat 1, 2, 3, 4, 5, 6, 7, 8, 9.
 * https://oeis.org/A177274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, easy, nonn
 * Formula:   a(n) = (n mod 9)+1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)(n % 9);
    return r + 1;
}
