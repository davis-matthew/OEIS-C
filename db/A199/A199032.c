/*
 * A199032  a(n) = 3*12^n-1.
 * https://oeis.org/A199032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*12^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)3;
    for (idx_t i = 0; i < n; ++i)
        result *= (seq_t)12;
    result -= (seq_t)1;
    return result;
}
