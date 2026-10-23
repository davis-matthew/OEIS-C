/*
 * A010144  Continued fraction for sqrt(59).
 * https://oeis.org/A010144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 8261, u16 n <= 8261, u32 n <= 8261, u64 n <= 8261,
 *            u128 n <= 8261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction for sqrt(59)
   a(0) = 7
   for n >= 1: a(n) = {1,2,7,2,1,14}[(n-1) mod 6] */
seq_t a(idx_t n)
{
    if (n == 0)
        return 7;
    static const seq_t pat[6] = {1, 2, 7, 2, 1, 14};
    return pat[(n - 1) % 6];
}
