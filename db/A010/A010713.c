/*
 * A010713  Period 2: repeat (4,8).
 * https://oeis.org/A010713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 - 2*(-1)^n (i.e., a(n) = 4 if n is even, 8 if n is odd)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the parity of n to pick the correct value */
    if ((n & 1) == 0)
        return (seq_t)4;
    else
        return (seq_t)8;
}
