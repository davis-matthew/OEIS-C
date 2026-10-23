/*
 * A017239  a(n) = (9*n + 6)^7.
 * https://oeis.org/A017239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 6)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9 * (seq_t)n + (seq_t)6;
    seq_t res = base;
    for (int i = 1; i < 7; ++i) {
        res *= base;
    }
    return res;
}
