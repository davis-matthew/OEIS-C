/*
 * A128964  a(n) = (n^3-n)*6^n.
 * https://oeis.org/A128964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3-n)*6^n = n*(n-1)*(n+1)*6^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; the check is only for safety. */
    if (n < 1)
        return 0;

    /* Convert the index to seq_t. */
    seq_t N = (seq_t)n;

    /* Compute 6^n by repeated multiplication. */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow6 *= (seq_t)6;

    /* n*(n-1)*(n+1) */
    seq_t prod = N * (N - (seq_t)1) * (N + (seq_t)1);

    /* Final value */
    return prod * pow6;
}
