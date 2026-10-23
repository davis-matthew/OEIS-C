/*
 * A021670  Decimal expansion of 1/666.
 * https://oeis.org/A021670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(n)=(n%3==2?1:(n%3==0?5:0)) for n>=2, i.e. the
 *            digits of 1/666
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }
    idx_t r = n % 3;
    if (r == 2) {
        return 1;
    }
    if (r == 0) {
        return 5;
    }
    return 0;
}
