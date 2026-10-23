/*
 * A017567  a(n) = (12*n + 3)^11.
 * https://oeis.org/A017567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 3)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 265,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (12*n + 3) in the seq_t type. */
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)3;

    /* Exponentiate the base by repeated multiplication. */
    seq_t res = 1;
    for (int i = 0; i < 11; ++i)
    {
        res *= base;
    }
    return res;
}
