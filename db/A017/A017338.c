/*
 * A017338  a(n) = (10*n + 5)^10.
 * https://oeis.org/A017338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base;
    /* compute 10*n+5 in seq_t to avoid overflow of idx_t */
    base = (seq_t)10 * (seq_t)n + 5;
    seq_t res = 1;
    /* multiply base ten times to obtain the 10th power */
    for (int i = 0; i < 10; ++i)
        res *= base;
    return res;
}
