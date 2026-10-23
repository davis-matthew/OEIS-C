/*
 * A014030  Inverse of 21st cyclotomic polynomial.
 * https://oeis.org/A014030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) repeats with period 21: a(0)=1, a(1)=1, a(2)=1, a(3)=0,
 *            a(4)=0, a(5)=0, a(6)=0, a(7)=-1, a(8)=-1, a(9)=-1, a(10)=0,
 *            a(11)=0, a(12)=0, a(13)=0, a(14)=0, a(15)=0, a(16)=0, a(17)=0,
 *            a(18)=0, a(19)=0, a(20)=0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[21] = {1,1,1,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0};
    return period[n % 21];
}
