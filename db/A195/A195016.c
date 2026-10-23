/*
 * A195016  a(n) = (n*(5*n+7) - (-1)^n + 1)/2.
 * https://oeis.org/A195016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(5*n+7) - (-1)^n + 1)/2
 * Range:     u8 n <= 6, u16 n <= 160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t prod = x * (5 * x + 7);  /* n * (5n + 7)  */
    if ((n & 1) == 0)            /* n even   */
        return prod / 2;         /* a(n) = n*(5n+7)/2 */
    else
        return (prod + 2) / 2;   /* a(n) = [n*(5n+7)+2]/2 */
}
