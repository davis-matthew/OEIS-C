/*
 * A123169  Continued fraction for
 *          sqrt(1/2)*(exp(sqrt(1/2))-1)/(exp(sqrt(1/2))+1).
 * https://oeis.org/A123169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2*n)=16*n-12, a(2*n+1)=8*n-2
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    seq_t n_s = (seq_t)n;
    if ((n_s & 1) == 0)       /* even: n = 2*k  */
    {
        seq_t k = n_s >> 1;   /* k = n/2  */
        return (seq_t)16 * k - (seq_t)12;
    }
    else                      /* odd: n = 2*k+1 */
    {
        seq_t k = (n_s - (seq_t)1) >> 1;   /* k = (n-1)/2 */
        return (seq_t)8 * k - (seq_t)2;
    }
}
