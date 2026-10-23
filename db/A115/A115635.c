/*
 * A115635  Periodic {1,1,-5,0,1,-3,-1,0,-3,1,-1,-4}.
 * https://oeis.org/A115635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = periodic sequence {1,1,-5,0,1,-3,-1,0,-3,1,-1,-4} (period
 *            12)
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[12] = {1, 1, -5, 0, 1, -3, -1, 0, -3, 1, -1, -4};
    return period[(int)(n % 12)];
}
