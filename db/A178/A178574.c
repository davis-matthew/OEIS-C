/*
 * A178574  a(n) = 2*n*(9*n-1).
 * https://oeis.org/A178574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(9*n-1)
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 2 * nn * ((seq_t)9 * nn - 1);
}
