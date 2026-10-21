/*
 * A084630  a(n) = floor(C(n+7,7)/C(n+5,5)).
 * https://oeis.org/A084630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor( n*(n+13)/42 )
 * Range:     u8 n <= 97, u16 n <= 1652, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)1 + (nn * (nn + (seq_t)13)) / (seq_t)42;
}
