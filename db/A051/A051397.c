/*
 * A051397  a(n) = (2*n-2)*(2*n-1)*a(n-1)+1.
 * https://oeis.org/A051397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-2)*(2*n-1)*a(n-1)+1 Initial value: a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)0;   /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* coefficient = (2*i-2)*(2*i-1) */
        seq_t two_i = (seq_t)2 * (seq_t)i;
        seq_t coef   = (two_i - (seq_t)2) * (two_i - (seq_t)1);
        result = coef * result + (seq_t)1;
    }
    return result;
}
