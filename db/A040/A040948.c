/*
 * A040948  Continued fraction for sqrt(980).
 * https://oeis.org/A040948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 31 if n==0, otherwise a periodic sequence for sqrt(980)
 *            with period length 10: [3,3,1,1,2,1,1,3,3,62]. So
 *            a(1)=3,a(2)=3,a(3)=1,a(4)=1,a(5)=2,a(6)=1,a(7)=1,a(8)=3,a(9)=3,a(10)=62,
 *            and the pattern repeats
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[10] = {3,3,1,1,2,1,1,3,3,62};
    if (n == 0)
        return 31;
    return period[(n-1) % 10];
}
