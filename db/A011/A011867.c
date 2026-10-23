/*
 * A011867  a(n) = floor(n*(n-1)/14).
 * https://oeis.org/A011867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (n - 1) / 14)
 * Range:     u8 n <= 16, u16 n <= 957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1);
    return prod / (seq_t)14;
}
