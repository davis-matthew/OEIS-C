/*
 * A131291  Period 9: repeat [5, 4, 5, 3, 4, 3, 5, 4, 5].
 * https://oeis.org/A131291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period[n mod 9], period = {5,4,5,3,4,3,5,4,5}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[9] = {5,4,5,3,4,3,5,4,5};
    return period[n % 9];
}
