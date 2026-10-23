/*
 * A196258  a(n) = 11^n*n!.
 * https://oeis.org/A196258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 11^n * n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        result *= (seq_t)i;      /* multiply by i! part */
        result *= (seq_t)11;     /* multiply by 11^n part by one factor each iteration */
    }
    return result;
}
