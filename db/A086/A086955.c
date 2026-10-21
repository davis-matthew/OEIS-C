/*
 * A086955  a(n) = n^2 + 2*n + 2 - (-1)^n.
 * https://oeis.org/A086955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2*n + 2 - (-1)^n
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^2 + 2*n + 2  in seq_t to avoid overflow of idx_t   */
    seq_t val = (seq_t)n * (seq_t)n + (seq_t)2 * (seq_t)n + (seq_t)2;

    /* adjust by subtracting (-1)^n :
       if n even  -> (-1)^n = 1  => subtract 1
       if n odd   -> (-1)^n = -1 => subtract(-1) = add 1     */
    if ((n & 1) == 0)
        val -= (seq_t)1;
    else
        val += (seq_t)1;

    return val;
}
