/*
 * A157653  a(n) = 80000*n^2 - 39200*n + 4801.
 * https://oeis.org/A157653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80000*n^2 - 39200*n + 4801
 * Range:     u8 none, u16 n <= 1, u32 n <= 231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)80000 * nn * nn
         - (seq_t)39200 * nn
         + (seq_t)4801;
}
