/*
 * A017625  a(n) = (12*n + 8)^9.
 * https://oeis.org/A017625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 8)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 1592,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 12*n + 8 */
    seq_t base = (seq_t)12 * n + 8;

    /* Compute base raised to the 9th power by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < 9; ++i)
        result *= base;

    return result;
}
