/*
 * A017110  a(n) = (8*n+3)^10.
 * https://oeis.org/A017110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 10, u128 n <= 891,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 8*n + 3, computed in seq_t to avoid overflow on idx_t */
    seq_t base = (seq_t)n * 8 + 3;
    seq_t result = 1;
    for (idx_t i = 0; i < 10; ++i)
        result *= base;
    return result;
}
