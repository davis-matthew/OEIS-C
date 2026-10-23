/*
 * A017492  a(n) = (11*n + 8)^8.
 * https://oeis.org/A017492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n + 8;
    seq_t result = base;
    for (idx_t i = 1; i < 8; ++i)
        result *= base;
    return result;
}
