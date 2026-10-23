/*
 * A016796  a(n) = (3*n + 2)^8.
 * https://oeis.org/A016796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (3*n + 2) in seq_t to avoid overflow */
    seq_t base = ((seq_t)n) * 3 + 2;

    /* Compute base^8 by repeated multiplication */
    seq_t result = 1;
    idx_t i;
    for (i = 0; i < 8; ++i)
    {
        result *= base;
    }

    return result;
}
