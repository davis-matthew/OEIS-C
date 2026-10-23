/*
 * A017240  a(n) = (9*n + 6)^8.
 * https://oeis.org/A017240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 27, u128 n <= 7281,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t base = sn * 9 + 6;
    seq_t result = base;
    for (idx_t i = 1; i < 8; ++i)
        result *= base;
    return result;
}
