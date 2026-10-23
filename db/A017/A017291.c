/*
 * A017291  a(n) = (10*n + 1)^11.
 * https://oeis.org/A017291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 318,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 10 + 1;
    seq_t res = 1;
    for (int i = 0; i < 11; ++i)
        res *= base;
    return res;
}
