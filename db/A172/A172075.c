/*
 * A172075  a(n) = n*(n+1)*(9*n^2 - n - 5)/6.
 * https://oeis.org/A172075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(9*n^2 - n - 5)/6
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 192, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                /* Convert index to seq_t */
    seq_t term = N * (N + 1);          /* n*(n+1) */
    term = term * ( (seq_t)9 * N * N - N - 5 ); /* *(9*n^2 - n -5) */
    return term / 6;                  /* divide by 6 (exact division) */
}
