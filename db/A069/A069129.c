/*
 * A069129  Centered 16-gonal numbers.
 * https://oeis.org/A069129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 8*n*n - 8*n + 1
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    const seq_t c8 = (seq_t)8;
    return c8 * nn * nn - c8 * nn + (seq_t)1;
}
