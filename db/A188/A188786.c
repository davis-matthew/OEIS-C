/*
 * A188786  Number of 3-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*n^4 - 112*n^3 + 48*n^2 (for n > 1; a(1) = 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          // n^2
    seq_t n3 = n2 * N;         // n^3
    seq_t n4 = n3 * N;         // n^4
    seq_t term = (seq_t)56 * n4
               - (seq_t)112 * n3
               + (seq_t)48 * n2;
    return term;
}
