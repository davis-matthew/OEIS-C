/*
 * A175631  a(n) = (n-th pentagonal number) modulo (n-th triangular number).
 * https://oeis.org/A175631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(3*n-1)/2) mod (n*(n+1)/2)
 * Range:     u8 n <= 13, u16 n <= 247, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t pent = nn * ((seq_t)3 * nn - 1) / 2;
    seq_t tri  = nn * (nn + 1) / 2;
    return pent % tri;
}
