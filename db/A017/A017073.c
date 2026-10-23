/*
 * A017073  a(n) = (8*n)^9.
 * https://oeis.org/A017073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 17, u128 n <= 2389,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * (seq_t)n;
    seq_t res  = 1;
    for (idx_t i = 0; i < 9; ++i)
        res *= base;
    return res;
}
