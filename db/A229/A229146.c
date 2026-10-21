/*
 * A229146  a(n) = n^3*(5*n+3)/2.
 * https://oeis.org/A229146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(5*n+3)/2
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 203, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t temp1 = tn * tn * tn;               /* n^3 */
    seq_t temp2 = ( (seq_t)5 ) * tn + (seq_t)3; /* 5*n + 3 */
    if ((temp1 & 1) == 0)                     /* divide the even factor by 2 first */
        temp1 >>= 1;
    else
        temp2 >>= 1;
    return temp1 * temp2;
}
