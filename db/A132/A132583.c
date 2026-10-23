/*
 * A132583  a(n) = n 2's sandwiched between two 1's.
 * https://oeis.org/A132583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1)+11, a(0)=11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)11;
    for (idx_t i = 1; i <= n; ++i)
        result = result * 10 + 11;
    return result;
}
