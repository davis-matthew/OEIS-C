/*
 * A016903  a(n) = (5*n + 4)^7.
 * https://oeis.org/A016903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 112, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)n * 5 + 4;
    seq_t result = base;
    for (int i = 1; i < 7; ++i)
        result *= base;
    return result;
}
