/*
 * A099721  a(n) = n^2*(2*n+1).
 * https://oeis.org/A099721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2*n+1)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t two_ns = ns + ns;          /* 2*n */
    return ns * ns * (two_ns + (seq_t)1);  /* n^2 * (2*n + 1) */
}
