/*
 * A017525  a(n) = (12*n)^5.
 * https://oeis.org/A017525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 594, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t base = ((seq_t)12 * tn);   /* 12*n */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
    {
        res *= base;
    }
    return res;
}
