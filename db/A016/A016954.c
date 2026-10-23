/*
 * A016954  a(n) = (6n+3)^10.
 * https://oeis.org/A016954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n+3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 1188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6 + 3;
    seq_t result = 1;
    int i;
    for (i = 0; i < 10; ++i)
        result *= base;
    return result;
}
