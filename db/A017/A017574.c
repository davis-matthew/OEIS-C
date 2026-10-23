/*
 * A017574  a(n) = (12*n + 4)^6.
 * https://oeis.org/A017574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 135, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * n + (seq_t)4;
    seq_t res  = (seq_t)1;
    for (int i = 0; i < 6; ++i)
        res *= base;
    return res;
}
