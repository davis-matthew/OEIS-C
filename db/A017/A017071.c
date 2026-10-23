/*
 * A017071  a(n) = (8*n)^7.
 * https://oeis.org/A017071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 70, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (8*n)^7 using seq_t arithmetic only. */
    seq_t base = (seq_t)8 * (seq_t)n;
    seq_t res  = (seq_t)1;
    for (idx_t i = 0; i < 7; ++i)
        res *= base;
    return res;
}
