/*
 * A063492  a(n) = (2*n - 1)*(11*n^2 - 11*n + 6)/6.
 * https://oeis.org/A063492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*(11*n^2 - 11*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 723, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                     /* cast the index to seq_t */
    seq_t part1 = (N << 1) - 1;             /* 2*n - 1 */
    seq_t n2   = N * N;                     /* n^2 */
    seq_t part2 = 11 * n2 - 11 * N + 6;     /* 11*n^2 - 11*n + 6 */
    seq_t prod  = part1 * part2;            /* (2*n - 1)*(11*n^2 - 11*n + 6) */
    return prod / 6;                        /* divide by 6 */
}
