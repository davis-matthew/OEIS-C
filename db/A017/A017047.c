/*
 * A017047  a(n) = (7*n + 5)^7.
 * https://oeis.org/A017047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^7
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 80, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * (seq_t)7 + (seq_t)5;
    seq_t res = x;
    for (idx_t i = 1; i < 7; ++i)
        res *= x;
    return res;
}
