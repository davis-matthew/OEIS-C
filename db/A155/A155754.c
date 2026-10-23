/*
 * A155754  A variation on 10^n mod 19.
 * https://oeis.org/A155754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-9); initial values: a(0)=1, a(1)=-9, a(2)=5,
 *            a(3)=-7, a(4)=6, a(5)=3, a(6)=-8, a(7)=-4, a(8)=-2
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {1, -9, 5, -7, 6, 3, -8, -4, -2};
    idx_t m = n % 18;          /* 18 = 2*9, period of the sequence */
    if (m < 9)
        return init[m];
    else
        return -init[m - 9];
}
