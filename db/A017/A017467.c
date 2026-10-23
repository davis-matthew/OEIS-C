/*
 * A017467  a(n) = (11*n + 6)^7.
 * https://oeis.org/A017467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 50, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 11 + 6;
    seq_t result = 1;
    /* multiply base seven times */
    for (unsigned i = 0; i < 7; ++i)
        result *= base;
    return result;
}
