/*
 * A040616  Continued fraction for sqrt(642).
 * https://oeis.org/A040616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(642).
   The first term is the integer part of sqrt(642), 25.
   Subsequent terms repeat with period [2,1,24,1,2,50]. */
static const seq_t period[6] = { 2, 1, 24, 1, 2, 50 };

seq_t a(idx_t n)
{
    if (n == 0)
        return 25;                 /* a(0) = 25 */
    return period[(n - 1) % 6];       /* a(1..) repeat every 6 terms */
}
