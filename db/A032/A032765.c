/*
 * A032765  a(n) = floor(n*(n+1)*(n+2) / (n + n+1 + n+2)) = floor(n*(n+2)/3).
 * https://oeis.org/A032765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+2)/3)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the output type before performing arithmetic */
    seq_t nn = (seq_t)n;
    /* n*(n+2) is always divisible by 3 for integer n, so integer division gives the floor. */
    return (nn * (nn + (seq_t)2)) / (seq_t)3;
}
