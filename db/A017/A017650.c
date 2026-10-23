/*
 * A017650  a(n) = (12*n + 10)^10.
 * https://oeis.org/A017650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 10)^10
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)10;
    seq_t result = 1;
    for (idx_t i = 0; i < 10; ++i)
        result *= base;
    return result;
}
