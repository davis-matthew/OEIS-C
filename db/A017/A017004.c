/*
 * A017004  a(n) = (7*n + 1)^12.
 * https://oeis.org/A017004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 232,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)n * 7 + 1; /* (7*n + 1) as a seq_t value   */
    seq_t result = 1;
    /* Multiply 12 times: result = base^12 */
    for (int i = 0; i < 12; ++i)
        result *= base;
    return result;
}
