/*
 * A073096  Maximal element in continued fraction for s(n) = sum(
 *          k>=n,1/2^(2^k) ).
 * https://oeis.org/A073096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 for n = 0 or 1; otherwise a(n) = 2^(2^n) + 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two special cases */
    if (n == 0 || n == 1)
        return (seq_t)6;

    /* start with 2^1 = 2 */
    seq_t val = (seq_t)2;

    /* repeatedly square n times: after k squarings val = 2^(2^k) */
    for (idx_t i = 1; i <= n; ++i)
        val = val * val;

    /* add the constant 2 */
    return val + (seq_t)2;
}
