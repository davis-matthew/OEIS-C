/*
 * A008402  Crystal ball sequence for {E_6}* lattice.
 * https://oeis.org/A008402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1 + 3*n*(n+1)*(n^2+n+1)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;          /* n */
    seq_t N1 = N + 1;             /* n + 1 */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N2N = N2 + N;           /* n^2 + n */
    seq_t t = N2N + 1;            /* n^2 + n + 1 */
    seq_t t2 = t * t;             /* (n^2 + n + 1)^2 */
    seq_t prod = N * N1;          /* n * (n + 1) */
    seq_t prod2 = prod * t2;      /* n*(n+1)*(n^2+n+1)^2 */
    return prod2 * (seq_t)3 + 1;  /* 1 + 3 * above */
}
