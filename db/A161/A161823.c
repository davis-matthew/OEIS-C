/*
 * A161823  Among any n lines on the plane, there exists a pair at an angle
 *          not more than a(n) degrees.
 * https://oeis.org/A161823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(180/n)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)180;           /* undefined for offset < 2, but safe */
    idx_t res = ((idx_t)180 + n - 1) / n;
    return (seq_t)res;
}
