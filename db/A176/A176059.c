/*
 * A176059  Periodic sequence: Repeat 3, 2.
 * https://oeis.org/A176059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, cons, nonn, easy
 * Formula:   a(n) = (5 + (-1)^n)/2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)2 : (seq_t)3;
}
