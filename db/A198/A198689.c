/*
 * A198689  a(n) = 8*7^n-1.
 * https://oeis.org/A198689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*7^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
        result *= 7;
    result *= 8;
    result -= 1;
    return result;
}
