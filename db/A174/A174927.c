/*
 * A174927  Periodic sequence: repeat (1,64).
 * https://oeis.org/A174927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, easy, nonn
 * Formula:   a(n) = 1 + 63 * (n mod 2) i.e., a(n) = (65 - 63*(-1)^n)/2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ternary branch-free: return 1 when n is even, 64 when n is odd */
    return (seq_t)(1 + 63 * (n & 1));
}
