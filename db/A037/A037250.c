/*
 * A037250  a(n) = n^2*(n^2 + 1)*(n-1).
 * https://oeis.org/A037250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 + 1)*(n-1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t result = N2 * (N2 + 1);
    result *= (N - 1);
    return result;
}
