/*
 * A017373  a(n) = (10*n + 8)^9.
 * https://oeis.org/A017373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 13, u128 n <= 1910,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base value (10*n + 8) in seq_t to avoid intermediate overflow. */
    seq_t base = (seq_t)n * 10 + 8;

    /* Compute the 9th power by repeated multiplication. */
    seq_t result = 1;
    int i;
    for (i = 0; i < 9; ++i)
        result *= base;

    return result;
}
