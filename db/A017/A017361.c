/*
 * A017361  a(n) = (10*n + 7)^9.
 * https://oeis.org/A017361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 13, u128 n <= 1910,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)10 * n + (seq_t)7;
    seq_t result = base;
    for (idx_t i = 1; i < 9; ++i)
        result *= base;
    return result;
}
