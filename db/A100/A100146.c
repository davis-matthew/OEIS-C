/*
 * A100146  Structured great rhombicubeoctahedral numbers.
 * https://oeis.org/A100146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(80*n^3 - 102*n^2 + 28*n)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 553, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term. For n < 1 the offset does not apply. */
    if (n < 1)
        return 0;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t term = 80 * N3 - 102 * N2 + 28 * N;
    return term / 6;
}
