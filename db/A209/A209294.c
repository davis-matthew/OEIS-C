/*
 * A209294  a(n) = (7*n^2 - 7*n + 4)/2.
 * https://oeis.org/A209294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 - 7*n + 4)/2
 * Range:     u8 n <= 6, u16 n <= 136, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = 7 * (nn * (nn - 1)) + 4;   /* 7*n*(n-1) + 4  */
    return num / 2;
}
