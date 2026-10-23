/*
 * A017302  a(n) = (10*n + 2)^10.
 * https://oeis.org/A017302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 10*n + 2 */
    seq_t base = ((seq_t)10 * (seq_t)n) + (seq_t)2;

    /* Compute base^10 by repeated multiplication */
    seq_t res = 1;
    for (idx_t i = 0; i < 10; ++i)
        res *= base;

    return res;
}
