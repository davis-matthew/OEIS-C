/*
 * A033573  a(n) = (2*n+1)*(9*n+1).
 * https://oeis.org/A033573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(9*n+1)
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;
    seq_t term1 = (2 * un) + 1;
    seq_t term2 = (9 * un) + 1;
    return term1 * term2;
}
