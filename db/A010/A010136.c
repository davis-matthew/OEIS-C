/*
 * A010136  Continued fraction for sqrt(46).
 * https://oeis.org/A010136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9706, u16 n <= 9706, u32 n <= 9706, u64 n <= 9706,
 *            u128 n <= 9706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(46):
   a(0) = 6
   a(n) = period[(n-1) % 12] for n >= 1,
   where period = {1, 3, 1, 1, 2, 6, 2, 1, 1, 3, 1, 12}
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 6;
    static const seq_t period[12] = {1, 3, 1, 1, 2, 6, 2, 1, 1, 3, 1, 12};
    return period[(n - 1) % 12];
}
