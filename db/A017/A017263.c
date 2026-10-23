/*
 * A017263  a(n) = (9*n + 8)^7.
 * https://oeis.org/A017263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 61, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 9*n + 8 */
    seq_t base = ((seq_t)9 * n) + (seq_t)8;
    /* Raise to the 7th power by repeated multiplication */
    seq_t r = 1;
    for (int i = 0; i < 7; ++i)
        r *= base;
    return r;
}
