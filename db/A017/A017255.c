/*
 * A017255  a(n) = (9*n + 7)^11.
 * https://oeis.org/A017255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 7)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 352,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 9u + 7u;
    seq_t result = 1;
    int i;
    for (i = 0; i < 11; ++i)
        result *= base;
    return result;
}
