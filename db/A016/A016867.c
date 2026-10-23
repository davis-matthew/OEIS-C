/*
 * A016867  a(n) = (5*n + 1)^7.
 * https://oeis.org/A016867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 112, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * n + 1;
    seq_t res = 1;
    for (int i = 0; i < 7; ++i) {
        res *= base;
    }
    return res;
}
