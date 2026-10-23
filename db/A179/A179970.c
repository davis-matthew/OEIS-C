/*
 * A179970  Numbers such that in base-4 representation all sums of two
 *          adjacent digits are odd.
 * https://oeis.org/A179970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 4*a(floor((n-1)/2)) - (a(floor((n-1)/2)) mod 2) + 1 +
 *            2*(1 - (n mod 2)) for n > 4, with a(1)=0, a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 46, u16 n <= 766, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;

    idx_t x = (n - 1) / 2;
    seq_t m = a(x);
    seq_t res = (seq_t)4 * m - (m & 1) + 1;
    if ((n & 1) == 0) res += 2;
    return res;
}
