/*
 * A034866  a(n) = n!*(n-4)/2, n > 4, and a(4) = 4.
 * https://oeis.org/A034866
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n-4)/2, n >4, a(4)=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4) {
        return (seq_t)4;
    }
    if (n < 4) {
        return (seq_t)0;
    }
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return fact * (seq_t)(n - 4) / 2;
}
