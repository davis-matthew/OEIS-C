/*
 * A082644  a(n) = floor(n*(n-1/2)/2).
 * https://oeis.org/A082644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(2*n-1)/4) = floor(n*(n-1/2)/2)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;               /* work in seq_t */
    seq_t tmp  = (nn << 1) - (seq_t)1;   /* tmp = 2*n - 1 */
    seq_t num  = nn * tmp;               /* num = n * (2*n - 1) */
    return num >> 2;                     /* divide by 4 (floor) */
}
