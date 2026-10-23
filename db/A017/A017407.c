/*
 * A017407  a(n) = (11*n + 1)^7.
 * https://oeis.org/A017407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 51, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 11*n + 1 */
    seq_t base = (seq_t)n * (seq_t)11 + (seq_t)1;

    /* compute base^7 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 7; ++i) {
        result *= base;
    }
    return result;
}
