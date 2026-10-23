/*
 * A017213  a(n) = (9*n + 4)^5.
 * https://oeis.org/A017213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 791, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = ((seq_t)9 * n) + (seq_t)4;
    seq_t res = (seq_t)1;
    for (int i = 0; i < 5; ++i)
        res *= x;
    return res;
}
