/*
 * A010808  20th powers: a(n) = n^20.
 * https://oeis.org/A010808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^20
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t base = (seq_t)n;
    for (int i = 0; i < 20; ++i)
        res *= base;
    return res;
}
