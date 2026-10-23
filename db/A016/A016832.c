/*
 * A016832  a(n) = (4*n + 2)^8.
 * https://oeis.org/A016832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 2)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 63, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 4*n + 2 with seq_t type to avoid overflow */
    seq_t base = (seq_t)4 * (seq_t)n + 2;
    /* Raise base to the 8th power by repeated multiplication */
    seq_t result = base;
    for (int i = 1; i < 8; ++i)
        result *= base;
    return result;
}
