/*
 * A017587  a(n) = (12*n + 5)^7.
 * https://oeis.org/A017587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 5)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 12*n + 5 */
    seq_t x = (seq_t)n * (seq_t)12 + (seq_t)5;

    /* Compute x^7 */
    seq_t res = x;
    /* Multiply six more times to get the 7th power */
    for (int i = 1; i < 7; ++i)
        res *= x;

    return res;
}
