/*
 * A212741  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          max{w,x,y,z}>=2*min{w,x,y,z}.
 * https://oeis.org/A212741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15 + (-1)^n + 64*n + 6*(15 + (-1)^n)*n^2 + 64*n^3 +
 *            14*n^4)/16
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute powers of n */
    seq_t n2 = (seq_t)n * (seq_t)n;  /* n^2 */
    seq_t n3 = n2 * (seq_t)n;        /* n^3 */
    seq_t n4 = n3 * (seq_t)n;        /* n^4 */

    seq_t numerator;
    if ((n & 1) == 0)  /* n is even */
    {
        numerator = (seq_t)14 * n4
                  + (seq_t)64 * n3
                  + (seq_t)96 * n2
                  + (seq_t)64 * (seq_t)n
                  + (seq_t)16;
    }
    else  /* n is odd */
    {
        numerator = (seq_t)14 * n4
                  + (seq_t)64 * n3
                  + (seq_t)84 * n2
                  + (seq_t)64 * (seq_t)n
                  + (seq_t)14;
    }

    return numerator / (seq_t)16;
}
