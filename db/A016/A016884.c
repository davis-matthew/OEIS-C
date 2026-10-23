/*
 * A016884  a(n) = (5*n + 2)^12.
 * https://oeis.org/A016884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 5 + 2;
    seq_t result = 1;
    for (int i = 0; i < 12; ++i)
        result *= base;
    return result;
}
