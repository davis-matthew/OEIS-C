/*
 * A074171  a(1) = 1. For n >= 2, a(n) is either a(n-1)+n or a(n-1)-n; we use
 *          the minus sign only if a(n-1) is prime. E.g., since a(2)=3 is
 *          prime, a(3)=a(2)-3=0.
 * https://oeis.org/A074171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1, a(2) = 3; for n ≥ 3, a(n) = (n-3)*(n+4)/2
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)        return (seq_t)1;
    if (n == 2)        return (seq_t)3;
    seq_t x = (seq_t)n;
    return ((x - (seq_t)3) * (x + (seq_t)4)) / (seq_t)2;
}
