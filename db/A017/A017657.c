/*
 * A017657  a(n) = (12*n + 11)^5.
 * https://oeis.org/A017657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^5
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 593, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 12*n + 11 */
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)11;

    /* compute base^5 */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 5; ++i)
        result *= base;

    return result;
}
