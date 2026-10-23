/*
 * A016879  a(n) = (5*n + 2)^7.
 * https://oeis.org/A016879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 112, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * (seq_t)n + (seq_t)2;
    seq_t result = 1;
    for (idx_t i = 0; i < 7; ++i)
        result *= base;
    return result;
}
