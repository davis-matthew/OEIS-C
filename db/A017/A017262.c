/*
 * A017262  a(n) = (9*n + 8)^6.
 * https://oeis.org/A017262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 179, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base = base * 9 + 8;
    seq_t res = base;
    for (int i = 1; i < 6; ++i) {
        res *= base;
    }
    return res;
}
