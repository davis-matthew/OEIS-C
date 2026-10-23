/*
 * A109123  a(n) = 4*(n+1)^2*(n+3)^2*(5*n^2 + 20*n + 12).
 * https://oeis.org/A109123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n+1)^2*(n+3)^2*(5*n^2 + 20*n + 12)
 * Range:     u8 none, u16 n <= 2, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t n3 = n0 + 3;
    seq_t term1 = n1 * n1;
    seq_t term2 = n3 * n3;
    seq_t term3 = 5 * n0 * n0 + 20 * n0 + 12;
    return 4 * term1 * term2 * term3;
}
