/*
 * A176040  Periodic sequence: repeat (3,1).
 * https://oeis.org/A176040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, cons, easy, nonn, mult
 * Formula:   a(n) = 2 + (-1)^n (i.e., a(n) = 3 for even n, 1 for odd n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; n & 1 yields 0 for even, 1 for odd */
    return (seq_t)((n & 1) ? 1 : 3);
}
