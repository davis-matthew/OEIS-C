/*
 * A010686  Periodic sequence: repeat [1, 5].
 * https://oeis.org/A010686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 3 - 2*(-1)^n (i.e., 1 if n is even, 5 if n is odd)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n & 1) ? 5 : 1);
}
