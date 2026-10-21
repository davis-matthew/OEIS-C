/*
 * A065236  a(n) = (4*n)!*(n+1)!/(2*n)!.
 * https://oeis.org/A065236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)!*(n+1)!/(2*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t k;

    /* Compute (4*n)!/(2*n)! as the product of integers from 2*n+1 to 4*n */
    for (k = 2 * n + 1; k <= 4 * n; ++k)
        res *= (seq_t)k;

    /* Multiply by (n+1)! */
    for (k = 1; k <= n + 1; ++k)
        res *= (seq_t)k;

    return res;
}
