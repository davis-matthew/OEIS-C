/*
 * A176636  Periodic sequence: Repeat [57, 71].
 * https://oeis.org/A176636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, cofr, nonn
 * Formula:   a(n) = 64 - 7*(-1)^n
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)71 : (seq_t)57;
}
