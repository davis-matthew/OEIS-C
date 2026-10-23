/*
 * A040191  Continued fraction for sqrt(206).
 * https://oeis.org/A040191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 14; for n >= 1, a(n) repeats the period
 *            (2,1,5,14,5,1,2,28). That is, a(n) = period[(n-1) % 8] where
 *            period = {2,1,5,14,5,1,2,28}
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;
    static const seq_t period[8] = {2, 1, 5, 14, 5, 1, 2, 28};
    return period[(n - 1) % 8];
}
