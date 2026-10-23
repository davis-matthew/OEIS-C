/*
 * A140139  Binomial transform of [1, 1, 2, -3, 4, -5, 6, -7, ...].
 * https://oeis.org/A140139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n==1, 2 if n==2, otherwise 2*n-1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;
    if (n == (idx_t)2)
        return (seq_t)2;
    return (seq_t)n * (seq_t)2 - (seq_t)1;
}
