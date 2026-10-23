/*
 * A010204  Continued fraction for sqrt(153).
 * https://oeis.org/A010204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 12, for n >= 1 a(n) = [2,1,2,2,2,1,2,24][(n-1) mod 8]
 *            (periodic continued fraction of sqrt(153))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 12; /* the integer part of sqrt(153) */

    static const seq_t period[8] = { 2, 1, 2, 2, 2, 1, 2, 24 };
    return period[(n - 1) % 8];
}
