/*
 * A040142  Continued fraction for sqrt(155).
 * https://oeis.org/A040142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(155). a(0) = 12, thereafter
 *            periodic block [2,4,2,24]
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;

    /* period of length 4 */
    static const seq_t period[4] = { (seq_t)2, (seq_t)4, (seq_t)2, (seq_t)24 };

    /* (n-1) % 4 – 4 is a power of two, so bitwise AND is safe */
    idx_t m = (n - 1) & 3;

    return period[m];
}
