/*
 * A017555  a(n) = (12*n + 2)^11.
 * https://oeis.org/A017555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^11
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 265,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * n + 2;
    seq_t res = 1;
    for (idx_t i = 0; i < 11; ++i)
        res *= base;
    return res;
}
