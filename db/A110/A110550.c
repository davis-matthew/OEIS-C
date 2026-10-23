/*
 * A110550  Periodic {1,3,2,4,4,2,3,1}.
 * https://oeis.org/A110550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) +
 *            a(n-7); initial values: a(0)=1, a(1)=3, a(2)=2, a(3)=4, a(4)=4,
 *            a(5)=2, a(6)=3, a(7)=1 (alternatively, the sequence is periodic
 *            with period 8: 1,3,2,4,4,2,3,1)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic block of length 8 */
    static const seq_t period[8] = { 1, 3, 2, 4, 4, 2, 3, 1 };
    return period[n & 7];   /* n % 8 is equivalent here, & 7 is faster */
}
