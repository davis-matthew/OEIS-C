/*
 * A017435  a(n) = (11*n + 3)^11.
 * https://oeis.org/A017435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 289,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * 11 + 3;
    seq_t res  = 1;
    idx_t i;
    for (i = 0; i < 11; ++i)
        res *= base;
    return res;
}
