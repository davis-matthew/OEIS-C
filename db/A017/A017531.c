/*
 * A017531  a(n) = (12*n)^11.
 * https://oeis.org/A017531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 265,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)12 * (seq_t)n;
    seq_t result = 1;
    for (int i = 0; i < 11; ++i)
        result *= base;
    return result;
}
