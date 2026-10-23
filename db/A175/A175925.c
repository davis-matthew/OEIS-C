/*
 * A175925  a(n) = (2*n+1)*(n+1)!.
 * https://oeis.org/A175925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(n+1)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        fact *= (seq_t)i;
    fact *= (seq_t)(n + (idx_t)1);

    /* compute (2*n + 1) */
    seq_t two_n_plus1 = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* return product */
    return two_n_plus1 * fact;
}
