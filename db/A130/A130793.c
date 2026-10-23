/*
 * A130793  Periodic sequence with period 3: 1, 3, 5.
 * https://oeis.org/A130793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n mod 3) + 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;
    seq_t r3 = rn % (seq_t)3;
    return (2 * r3 + (seq_t)1);
}
