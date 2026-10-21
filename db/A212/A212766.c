/*
 * A212766  Number of (w,x,y,z) with all terms in {0,...,n}, w even and x odd.
 * https://oeis.org/A212766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((n+1)^2)/4) * (n+1)^2
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid implicit conversions during arithmetic */
    seq_t m = (seq_t)n + (seq_t)1;          /* m = n+1  */
    seq_t m2 = m * m;                       /* (n+1)^2   */
    seq_t quarter = m2 / ((seq_t)4);        /* floor((n+1)^2 / 4)  */
    return m2 * quarter;                    /* a(n) = m2 * floor(m2/4) */
}
