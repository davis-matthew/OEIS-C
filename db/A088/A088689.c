/*
 * A088689  Jacobsthal numbers modulo 3.
 * https://oeis.org/A088689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-6) with a(0)=0, a(1)=a(2)=1, a(3)=0, a(4)=a(5)=2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {0, 1, 1, 0, 2, 2};
    return pat[(unsigned)(n % 6U)];
}
