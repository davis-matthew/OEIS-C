/*
 * A237930  a(n) = 3^(n+1) + (3^n-1)/2.
 * https://oeis.org/A237930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(n)=3*a(n-1)+1 for n>0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 3;
    for (idx_t i = 1; i <= n; ++i)
        result = result * 3 + 1;
    return result;
}
