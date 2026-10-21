/*
 * A218753  a(n) = (49^n - 1)/48.
 * https://oeis.org/A218753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*a(n-1) + 1, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        result = result * (seq_t)49 + 1;
    }
    return result;
}
