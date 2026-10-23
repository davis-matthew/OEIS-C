/*
 * A128786  a(n) = n^2*7^n.
 * https://oeis.org/A128786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7^n */
    seq_t pow7 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow7 *= (seq_t)7;
    }
    seq_t nn = (seq_t)n;
    return pow7 * nn * nn;
}
