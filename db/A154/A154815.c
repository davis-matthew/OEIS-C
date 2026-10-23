/*
 * A154815  Period 6: repeat [8, 7, 4, 5, 2, 1].
 * https://oeis.org/A154815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n > 5; Initial terms: a(0)=8, a(1)=7, a(2)=4,
 *            a(3)=5, a(4)=2, a(5)=1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic sequence of length 6 */
    static const seq_t period[6] = {8, 7, 4, 5, 2, 1};
    return period[n % 6];
}
