/*
 * A017074  a(n) = (8*n)^10.
 * https://oeis.org/A017074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 10, u128 n <= 891,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)8;
    seq_t res = (seq_t)1;
    for (int i = 0; i < 10; ++i)
        res *= m;
    return res;
}
