/*
 * A040827  Continued fraction for sqrt(857).
 * https://oeis.org/A040827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 29; for n>0, a(n) is the (n-1)th element of the period
 *            [3,1,1,1,3,1,6,1,1,6,1,3,1,1,1,3,58] for the continued fraction
 *            of √857
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[17] = {3,1,1,1,3,1,6,1,1,6,1,3,1,1,1,3,58};
    if (n == 0)
        return 29;
    idx_t k = (n - 1) % 17;
    return period[k];
}
