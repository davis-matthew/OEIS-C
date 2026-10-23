/*
 * A198959  a(1) = 2, a(n+1) = a(n)^2 - n.
 * https://oeis.org/A198959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(n+1) = a(n)^2 - n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    seq_t result = (seq_t)2;
    for (idx_t i = 1; i < n; ++i)
        result = result * result - (seq_t)i;
    return result;
}
