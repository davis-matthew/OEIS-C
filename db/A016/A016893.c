/*
 * A016893  a(n) = (5*n+3)^9.
 * https://oeis.org/A016893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 5 + 3;
    seq_t result = 1;
    for (int i = 0; i < 9; i++)
        result *= base;
    return result;
}
