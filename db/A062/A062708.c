/*
 * A062708  Write 0,1,2,3,4,... in a triangular spiral; then a(n) is the
 *          sequence found by reading the terms along the line from 0 in the
 *          direction 0,2,...
 * https://oeis.org/A062708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n-5)/2
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term = (seq_t)9 * ns * ns - (seq_t)5 * ns;
    return term / 2;
}
