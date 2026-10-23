/*
 * A183873  a(n) = n-1+ceiling((1/5)n^2); complement of A183872.
 * https://oeis.org/A183873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + ceil((1/5)n^2)
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;            /* convert index to seq_t */
    seq_t sq   = nn * nn;             /* n^2          */
    seq_t ceil = (sq + 4) / 5;        /* ceil(n^2/5)  */
    return (seq_t)(n - 1) + ceil;     /* n-1 + ceil    */
}
