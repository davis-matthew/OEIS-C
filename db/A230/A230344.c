/*
 * A230344  Number of permutations of [2n+4] in which the longest increasing
 *          run has length n+4.
 * https://oeis.org/A230344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+10*n^2+30*n+29)*(2*n+4)!/(n+6)! for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    seq_t nn = (seq_t)n;
    /* polynomial part: n^3 + 10*n^2 + 30*n + 29 */
    seq_t res = nn * nn * nn
                + (seq_t)10 * nn * nn
                + (seq_t)30 * nn
                + (seq_t)29;

    /* multiply by the product (n+7)*(n+8)*...*(2*n+4) */
    for (idx_t i = n + 7; i <= 2 * n + 4; ++i) {
        res *= (seq_t)i;
    }
    return res;
}
