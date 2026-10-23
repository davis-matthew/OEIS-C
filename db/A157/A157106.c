/*
 * A157106  a(n) = 5651522*n^2 - 2541672*n + 285769.
 * https://oeis.org/A157106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5651522*n^2 - 2541672*n + 285769
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t res = (seq_t)5651522 * nn - (seq_t)2541672 * (seq_t)n + (seq_t)285769;
    return res;
}
