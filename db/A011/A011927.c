/*
 * A011927  a(n) = floor(n*(n-1)*(n-2)*(n-3)/17).
 * https://oeis.org/A011927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/17)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 468, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) /* terms for n = 0,1,2,3 are 0 */
        return 0;
    seq_t x = (seq_t)n;
    seq_t prod = x * (x - 1) * (x - 2) * (x - 3);
    return prod / (seq_t)17;
}
