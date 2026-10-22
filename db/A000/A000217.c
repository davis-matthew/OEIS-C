/*
 * A000217  Triangular numbers: a(n) = binomial(n+1,2) = n*(n+1)/2 = 0 + 1 + 2
 *          + ... + n.
 * https://oeis.org/A000217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 9231, u64 n <= 9231,
 *            u128 n <= 9231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (n % 2 == 0)
        return (m / 2) * (m + 1);
    return m * ((m + 1) / 2);
}
