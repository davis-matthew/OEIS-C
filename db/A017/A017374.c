/*
 * A017374  a(n) = (10*n + 8)^10.
 * https://oeis.org/A017374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)10 * n + (seq_t)8;
    seq_t res  = (seq_t)1;
    for (int i = 0; i < 10; ++i)
        res *= base;
    return res;
}
