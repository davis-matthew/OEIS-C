/*
 * A100157  Structured rhombic dodecahedral numbers (vertex structure 9).
 * https://oeis.org/A100157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n-1)*(4*n-1)/3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1171, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term = ns * (((seq_t)2 * ns) - 1) * (((seq_t)4 * ns) - 1);
    return term / ((seq_t)3);
}
