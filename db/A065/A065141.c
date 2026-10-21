/*
 * A065141  a(n) = (n+1)*2^n*(2*n)!.
 * https://oeis.org/A065141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * 2^n * (2*n)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                     /* start with 1 */
    idx_t two_n = 2 * n;                /* compute 2*n */

    /* compute (2*n)! */
    for (idx_t i = 2; i <= two_n; ++i)
        res *= (seq_t)i;

    /* multiply by (n+1) */
    res *= (seq_t)(n + 1);

    /* multiply by 2^n */
    for (idx_t i = 0; i < n; ++i)
        res *= 2;

    return res;
}
