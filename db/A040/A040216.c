/*
 * A040216  Continued fraction for sqrt(232).
 * https://oeis.org/A040216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=15, a(n)= periodic pattern [4,3,7,3,4,30] for n>=1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)15;
    static const seq_t periodic[6] = {
        (seq_t)4, (seq_t)3, (seq_t)7, (seq_t)3, (seq_t)4, (seq_t)30
    };
    return periodic[(n - 1) % 6];
}
