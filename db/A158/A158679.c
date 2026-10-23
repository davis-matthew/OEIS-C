/*
 * A158679  a(n) = 961*n^2 - 31.
 * https://oeis.org/A158679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 961*n*n - 31
 * Range:     u8 none, u16 n <= 8, u32 n <= 2114, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t val = (seq_t)961 * m * m;
    val -= (seq_t)31;
    return val;
}
