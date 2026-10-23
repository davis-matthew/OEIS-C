/*
 * A175822  Partial sums of ceiling(n^2/7).
 * https://oeis.org/A175822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(2*n^2 + n + 24)/42)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1307, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = round((2*n+1)*(n^2 + n + 12)/42) */

/* Computes the nth term of OEIS A175822: partial sums of ceiling(n^2/7). */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t safely */
    seq_t n1 = N + 1;            /* (n + 1) */
    seq_t tmp = (seq_t)2 * N * N; /* 2 * n^2, computed before adding n + 24 */
    tmp += N + (seq_t)24;        /* 2*n^2 + n + 24 */
    seq_t num = n1 * tmp;        /* (n + 1)*(2*n^2 + n + 24) */
    return num / 42;             /* floor division by 42 gives the term */
}
