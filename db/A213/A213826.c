/*
 * A213826  Principal diagonal of the convolution array A213825.
 * https://oeis.org/A213826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -n - 3*n^2 + 6*n^3
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* values for n < 1 are undefined */
    seq_t sn  = (seq_t)n;            /* convert index to seq_t */
    seq_t n2  = sn * sn;             /* n^2  */
    seq_t n3  = n2 * sn;             /* n^3  */
    return (seq_t)6 * n3 - (seq_t)3 * n2 - sn;
}
