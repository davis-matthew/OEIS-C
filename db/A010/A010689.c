/*
 * A010689  Periodic sequence: Repeat 1, 8.
 * https://oeis.org/A010689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, cons, easy
 * Formula:   a(n) = (9 - 7*(-1)^n)/2 (periodic 1, 8)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n & 1) ? 8 : 1);
}
