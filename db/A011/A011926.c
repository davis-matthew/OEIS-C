/*
 * A011926  a(n) = floor(n*(n-1)*(n-2)*(n-3)/16).
 * https://oeis.org/A011926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/16)
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4u)
        return 0;
    seq_t n0 = (seq_t)n;
    seq_t n1 = (seq_t)(n - 1u);
    seq_t n2 = (seq_t)(n - 2u);
    seq_t n3 = (seq_t)(n - 3u);
    return (n0 * n1 * n2 * n3) / (seq_t)16;
}
