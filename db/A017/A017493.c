/*
 * A017493  a(n) = (11*n + 8)^9.
 * https://oeis.org/A017493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type first */
    seq_t base = (seq_t)n * (seq_t)11 + (seq_t)8;
    seq_t result = 1;
    /* Exponent 9 is small; a simple repeated multiplication suffices */
    for (idx_t i = 0; i < 9; ++i)
        result *= base;
    return result;
}
