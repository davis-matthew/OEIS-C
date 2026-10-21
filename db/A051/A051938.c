/*
 * A051938  Truncated triangular numbers: a(n) = n*(n+1)/2 - 18.
 * https://oeis.org/A051938
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 - 18 (offset 6, so a(6)=3)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6)        /* undefined below the OEIS offset – return 0 */
        return 0;
    seq_t N = (seq_t)n;
    seq_t tri = N * (N + 1) / 2;
    return tri - (seq_t)18;
}
