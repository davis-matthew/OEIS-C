/*
 * A156844  a(n) = 279841*n^2 - 394634*n + 139128.
 * https://oeis.org/A156844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 279841*n^2 - 394634*n + 139128
 * Range:     u8 none, u16 n <= 1, u32 n <= 124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = 279841 * nn * nn;
    seq_t term2 = 394634 * nn;
    return term1 - term2 + 139128;
}
