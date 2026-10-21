/*
 * A230347  Number of permutations of [2n+7] in which the longest increasing
 *          run has length n+7.
 * https://oeis.org/A230347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+16*n^2+72*n+71)*(2*n+7)!/(n+9)! (n>0) For n=0
 *            a(0)=1. The factorial ratio simplifies to
 *            product_{k=n+10}^{2*n+7} k for n>=2, with the single exceptional
 *            case n=1 where a(1)=16
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)16;

    /* compute the polynomial factor */
    seq_t nn = (seq_t)n;
    seq_t poly = nn * nn * nn            /* n^3      */
               + (seq_t)16 * nn * nn      /* 16 n^2   */
               + (seq_t)72 * nn          /* 72 n     */
               + (seq_t)71;              /* + 71     */

    /* multiply by the remaining factorial ratio for n >= 2:
       product_{k=n+10}^{2*n+7} k
    */
    seq_t result = poly;
    for (idx_t k = n + 10; k <= 2 * n + 7; ++k)
        result *= (seq_t)k;

    return result;
}
