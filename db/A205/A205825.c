/*
 * A205825  a(n) = n!/ceiling(n/2)!.
 * https://oeis.org/A205825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / ⌈n/2⌉! (product from ⌈n/2⌉+1 to n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* start = ceil(n/2) + 1 = (n + 1)/2 + 1 */
    idx_t start = ((n + 1) / 2) + 1;
    seq_t res = 1;
    for (idx_t k = start; k <= n; ++k)
        res *= (seq_t)k;
    return res;
}
