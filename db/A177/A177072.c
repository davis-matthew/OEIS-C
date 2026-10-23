/*
 * A177072  a(n) = (9*n+2)*(9*n+7).
 * https://oeis.org/A177072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+2)*(9*n+7)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t a1 = 9 * tn + 2;
    seq_t a2 = 9 * tn + 7;
    return a1 * a2;
}
