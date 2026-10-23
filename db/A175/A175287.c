/*
 * A175287  Partial sums of ceiling(n^2/4).
 * https://oeis.org/A175287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1598, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175287: Partial sums of ceil(k^2/4)
   Formula: a(n) = floor((n+1)*(2*n^2 + n + 9) / 24)   (OEIS)
   The formula works for all non‑negative n. */
seq_t a(idx_t n)
{
    /* cast the index to seq_t before any arithmetic */
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;              /* n^2          */
    seq_t T   = (seq_t)2 * N2 + N + (seq_t)9;  /* 2*n^2 + n + 9 */
    seq_t prod = (N + (seq_t)1) * T;           /* (n+1)*T     */
    return prod / (seq_t)24;                   /* floor division */
}
