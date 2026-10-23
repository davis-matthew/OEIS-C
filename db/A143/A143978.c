/*
 * A143978  a(n) = floor(2*n*(n+1)/3).
 * https://oeis.org/A143978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n*(n+1)/3)
 * Range:     u8 n <= 10, u16 n <= 227, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but we safely handle n == 0. */
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + (seq_t)1) * (seq_t)2;
    return prod / (seq_t)3;
}
