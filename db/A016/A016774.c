/*
 * A016774  a(n) = (3*n)^10.
 * https://oeis.org/A016774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^10
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 28, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)3;
    seq_t res = 1;
    for (int i = 0; i < 10; ++i)
        res *= x;
    return res;
}
