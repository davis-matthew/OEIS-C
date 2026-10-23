/*
 * A017251  a(n) = (9*n+7)^7.
 * https://oeis.org/A017251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+7)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)9) * n + 7;
    seq_t res = 1;
    for (int i = 0; i < 7; ++i)
        res *= x;
    return res;
}
