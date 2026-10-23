/*
 * A135519  Generalized repunits in base 14.
 * https://oeis.org/A135519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) + 1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t result = (seq_t)1;         /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        result = result * 14 + 1;
    return result;
}
