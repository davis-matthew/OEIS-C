/*
 * A040610  Continued fraction for sqrt(636).
 * https://oeis.org/A040610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25, a(n)=period[(n-1)%12] for n>=1,
 *            period={4,1,1,3,3,12,3,3,1,1,4,50}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    static const seq_t period[12] = {4, 1, 1, 3, 3, 12, 3, 3, 1, 1, 4, 50};
    idx_t k = (n - 1) % 12;
    return period[k];
}
