/*
 * A017160  a(n) = (8*n + 7)^12.
 * https://oeis.org/A017160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 8*n + 7 in seq_t */
    seq_t base = (seq_t)n * (seq_t)8 + (seq_t)7;
    /* Compute base^12 by repeated multiplication */
    seq_t res = base;
    for (idx_t i = 1; i < 12; ++i)
    {
        res *= base;
    }
    return res;
}
