/*
 * A017314  a(n) = (10*n + 3)^10.
 * https://oeis.org/A017314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the base 10*n + 3 using seq_t arithmetic */
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)3;

    /* compute base^10 by repeated multiplication */
    seq_t result = 1;
    for (int i = 0; i < 10; ++i)
        result *= base;

    return result;
}
