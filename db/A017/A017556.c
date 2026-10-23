/*
 * A017556  a(n) = (12*n + 2)^12.
 * https://oeis.org/A017556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 3, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 12*n + 2 safely in seq_t */
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)2;
    seq_t result = (seq_t)1;

    /* exponentiation by repeated multiplication (12 times) */
    for (int i = 0; i < 12; ++i)
        result *= base;

    return result;
}
