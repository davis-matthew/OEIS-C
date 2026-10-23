/*
 * A017359  a(n) = (10*n + 7)^7.
 * https://oeis.org/A017359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 55, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (10*n + 7) as the base */
    seq_t m = (seq_t)n * 10 + 7;
    seq_t r = 1;
    /* raise m to the 7th power */
    for (int i = 0; i < 7; ++i)
        r *= m;
    return r;
}
