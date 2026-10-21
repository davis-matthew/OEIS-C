/*
 * A212700  a(n) = 5*n*6^(n-1).
 * https://oeis.org/A212700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*6^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for offset 1, but return 0 */

    /* compute 6^(n-1) iteratively */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow6 *= (seq_t)6;

    /* a(n) = 5 * n * 6^(n-1) */
    pow6 *= (seq_t)5;
    pow6 *= (seq_t)n;

    return pow6;
}
