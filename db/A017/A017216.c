/*
 * A017216  a(n) = (9*n + 4)^8.
 * https://oeis.org/A017216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 27, u128 n <= 7281,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)9 * (seq_t)n) + 4;
    seq_t result = 1;
    int i;
    for (i = 0; i < 8; ++i)
        result *= base;
    return result;
}
