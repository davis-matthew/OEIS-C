/*
 * A146761  Period 6: repeat [0, 0, 6, 6, 3, 3].
 * https://oeis.org/A146761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic with period 6: a(0)=0,a(1)=0,a(2)=6,
 *            a(3)=6,a(4)=3,a(5)=3, and a(n+6)=a(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    static const seq_t period[6] = {0, 0, 6, 6, 3, 3};
    return period[n % 6];
}
