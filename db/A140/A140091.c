/*
 * A140091  a(n) = 3*n*(n + 3)/2.
 * https://oeis.org/A140091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(n+3)/2
 * Range:     u8 n <= 11, u16 n <= 207, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n*(n+3)/2 first to keep products within range,
       then multiply by 3.  This works for any build where
       seq_t is an unsigned integer type or arbitrary precision. */
    seq_t nn = (seq_t)n;
    seq_t temp = nn * (nn + 3) / 2;   /* divisible by 2 */
    return temp * 3;
}
