/*
 * A040344  Continued fraction for sqrt(364).
 * https://oeis.org/A040344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction of sqrt(364). a(0) = 19; for n >= 1,
   a(n) = period[(n-1) % 12] where period = {12,1,2,3,1,8,1,3,2,1,12,38}. */
seq_t a(idx_t n)
{
    if (n == 0) return 19;
    static const seq_t period[12] = {12, 1, 2, 3, 1, 8, 1, 3, 2, 1, 12, 38};
    idx_t i = (n - 1) % 12;
    return period[i];
}
