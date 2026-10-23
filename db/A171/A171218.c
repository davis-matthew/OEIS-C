/*
 * A171218  a(n) = Sum_{k=0..n} A109613(k)*A005843(n-k).
 * https://oeis.org/A171218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(2*n^2+3*n+4) - 3*(-1)^n + 3)/12
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1808, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;          /* n as seq_t */
    seq_t n2    = n_val * n_val;     /* n^2 */
    seq_t t     = (seq_t)2 * n2      /* 2*n^2 */
                + (seq_t)3 * n_val   /* +3*n */
                + (seq_t)4;          /* +4 */
    seq_t term  = (seq_t)2 * n_val * t; /* 2*n*t */

    /* Add 6 if n is odd, since -3*(-1)^n + 3 = 0 for even n, 6 for odd n */
    if (n & (idx_t)1)
        term += (seq_t)6;

    return term / (seq_t)12;
}
