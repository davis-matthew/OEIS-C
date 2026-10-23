/*
 * A175430  a(n) = (n-1)! * (n+1)!.
 * https://oeis.org/A175430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * (n+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* a(n) = (n-1)! * (n+1)! = fact^2 * n * (n+1) */
    seq_t res = fact;
    res *= fact;                /* fact squared */
    res *= (seq_t)n;            /* multiply by n */
    res *= ((seq_t)n + 1);      /* multiply by n+1 */
    return res;
}
