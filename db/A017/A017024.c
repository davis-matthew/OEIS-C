/*
 * A017024  a(n) = (7*n + 3)^8.
 * https://oeis.org/A017024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)^8
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 36, u128 n <= 9361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)7 * (seq_t)n) + (seq_t)3;
    seq_t res  = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        res *= base;
    return res;
}
