/*
 * A017504  a(n) = (11*n + 9)^8.
 * https://oeis.org/A017504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 9)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (11*n + 9) as seq_t */
    seq_t base = (seq_t)11 * n + 9;
    seq_t res  = 1;
    /* raise base to the 8th power */
    for (int i = 0; i < 8; ++i)
        res *= base;
    return res;
}
