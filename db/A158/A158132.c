/*
 * A158132  a(n) = 144n^2 + 2n.
 * https://oeis.org/A158132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 + 2*n
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 144 * tn * tn + 2 * tn;
}
