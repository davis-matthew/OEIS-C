/*
 * A116720  Number of permutations of length n which avoid the patterns 213,
 *          1234, 4312.
 * https://oeis.org/A116720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * a(0) = 1, * a(1) = 1, * a(2) = 2, * a(n) = 3*n^2 - 14*n
 *            + 20 for n >= 3
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t N = (seq_t)n;
    const seq_t S3  = (seq_t)3;
    const seq_t S14 = (seq_t)14;
    const seq_t S20 = (seq_t)20;

    return S3 * N * N - S14 * N + S20;
}
