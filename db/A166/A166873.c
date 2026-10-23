/*
 * A166873  a(n) = a(n-1) + 12*n for n > 1; a(1) = 1.
 * https://oeis.org/A166873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n^2 + 6*n - 11
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1, undefined below */
    seq_t N = (seq_t)n;
    return (seq_t)6 * N * N + (seq_t)6 * N - (seq_t)11;
}
