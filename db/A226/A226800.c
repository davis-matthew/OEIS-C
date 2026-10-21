/*
 * A226800  Column 5 of array in A226513.
 * https://oeis.org/A226800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^4+24*n^3+186*n^2+554*n+541)
 * Range:     u8 none, u16 n <= 4, u32 n <= 79, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t n1   = nseq + 1;
    seq_t P    = 1;
    P = P * nseq + 24;
    P = P * nseq + 186;
    P = P * nseq + 554;
    P = P * nseq + 541;
    return n1 * P;
}
