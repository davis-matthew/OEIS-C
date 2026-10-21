/*
 * A202141  a(n) = 13*n^2 - 16*n + 5.
 * https://oeis.org/A202141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2 - 16*n + 5
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t tn2 = tn * tn;
    seq_t res = (seq_t)13 * tn2 - (seq_t)16 * tn + (seq_t)5;
    return res;
}
