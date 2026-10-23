/*
 * A013830  a(n) = 4^(5*n + 1).
 * https://oeis.org/A013830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(5*n + 1) = 4 * 1024^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t result = 4;          /* a(0) */
    for (i = 0; i < n; ++i)    /* multiply by 1024 each time */
        result *= 1024;        /* result = result * 1024 */
    return result;
}
