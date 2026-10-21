/*
 * A204558  Row sums of the triangle A045975.
 * https://oeis.org/A204558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * (n^2 - (-1)^n) / 2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, so a(1) is the first term */
    seq_t sn = (seq_t)n;                  /* convert index to the sequence type   */
    seq_t n2 = sn * sn;                   /* n^2                                   */

    if ((n & 1) == 0)                    /* n even -> use (n^2-1)/2 factor       */
    {
        return (n2 / 2) * (n2 - 1);
    }
    else                                 /* n odd  -> use (n^2+1)/2 factor       */
    {
        return n2 * ((n2 + 1) / 2);
    }
}
