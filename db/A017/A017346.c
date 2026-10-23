/*
 * A017346  a(n) = (10*n + 6)^6.
 * https://oeis.org/A017346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 161, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * 10 + 6;
    seq_t result = 1;
    for (int i = 0; i < 6; ++i)
        result *= base;
    return result;
}
