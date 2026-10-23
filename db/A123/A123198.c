/*
 * A123198  a(n) = ((n+1)*(2*n-1))^2.
 * https://oeis.org/A123198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(2*n-1))^2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;          /* (0+1)*(2*0-1) = 1*(-1) = -1, squared → 1 */
    }
    seq_t s = (seq_t)n;
    seq_t x = s + (seq_t)1;       /* n+1  */
    seq_t y = s * ((seq_t)2) - (seq_t)1; /* 2*n-1   (safe for n>=1) */
    seq_t prod = x * y;           /* (n+1)*(2*n-1)  */
    return prod * prod;           /* square of the product */
}
