/*
 * A051443  a(n) = n^(n+1)*(n+1)^n.
 * https://oeis.org/A051443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+1)*(n+1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                /* a(0) is defined as 0 */

    seq_t res   = (seq_t)1;            /* start accumulator */
    seq_t baseN = (seq_t)n;

    /* compute n^(n+1) */
    for (idx_t i = (idx_t)0; i < n + (idx_t)1; ++i)
        res *= baseN;

    seq_t baseNp1 = (seq_t)(n + (idx_t)1);

    /* multiply by (n+1)^n */
    for (idx_t i = (idx_t)0; i < n; ++i)
        res *= baseNp1;

    return res;
}
