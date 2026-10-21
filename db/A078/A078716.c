/*
 * A078716  Sequence has period 9 and differences between successive terms are
 *          4, -3, 4, -3, 4, -3, 4, -3, -4.
 * https://oeis.org/A078716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = periodic sequence with period 9 a(1)=1, a(2)=5, a(3)=2,
 *            a(4)=6, a(5)=3, a(6)=7, a(7)=4, a(8)=8, a(9)=5, and a(n+9)=a(n)
 *            for all n≥1 (differences: 4, -3, 4, -3, 4, -3, 4, -3, -4)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t t[9] = {1,5,2,6,3,7,4,8,5};
    /* Accept any n>=0; behavior for n=0 is unspecified, we return t[8] (5) */
    idx_t r = (n == 0) ? 8 : (n - 1) % 9;
    return t[r];
}
