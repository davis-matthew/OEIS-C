/*
 * A059722  a(n) = n*(2*n^2 - 2*n + 1).
 * https://oeis.org/A059722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 2*n + 1)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                   /* cast index to seq_t */
    seq_t term = (seq_t)2 * N * N          /* 2*n^2          */
                - (seq_t)2 * N             /* - 2*n          */
                + (seq_t)1;                /* + 1            */
    return N * term;                      /* n * term       */
}
