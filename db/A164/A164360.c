/*
 * A164360  Period 3: repeat [5, 4, 3].
 * https://oeis.org/A164360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3), a(0)=5, a(1)=4, a(2)=3 (period 3: 5,4,3)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* lookup table for one period */
    static const seq_t period[3] = {5, 4, 3};
    return period[n % 3];
}
