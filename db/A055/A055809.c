/*
 * A055809  a(n) = T(n,n-4), array T as in A055807.
 * https://oeis.org/A055809
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 3*n - 22)/6 (for n > 4), a(4) = 1
 * Range:     u8 n <= 11, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: a(4) is the first term */
    if (n < 4)
        return 0;          /* undefined for n < 4, return 0 by convention */

    if (n == 4)
        return 1;

    seq_t N = (seq_t)n;

    /* Compute N * (N^2 + 3*N - 22) / 6 in a single expression.
       All operations are performed in seq_t; the formula guarantees
       that division by 6 yields an integer result. */
    seq_t result = N * (N * N + 3 * N - 22) / 6;

    return result;
}
