/*
 * A067624  a(n) = 2^(2*n)*(2*n)!.
 * https://oeis.org/A067624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n)*(2*n)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t m = 2 * n;               /* factorial part: (2*n)! */
    for (idx_t i = 2; i <= m; ++i)
        res *= (seq_t)i;
    for (idx_t i = 0; i < 2 * n; ++i) /* multiply by 2^(2*n) without overflow */
        res *= (seq_t)2;
    return res;
}
