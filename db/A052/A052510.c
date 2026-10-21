/*
 * A052510  Number of labeled planar binary trees with 2n-1 elements (external
 *          nodes or internal nodes).
 * https://oeis.org/A052510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1)/n * ( ((2*(n-1))!)/(n-1)! )^2 Implemented by
 *            iteratively building the product P(n) = ((2*(n-1))!)/(n-1)! =
 *            Π_{k=n}^{2n-2} k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value outside the offset */
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t P = 1;                   /* product for current i */
    for (idx_t i = 2; i <= n; ++i) {
        /* P(i) = P(i-1) * (2*i-3)*(2*i-2) / (i-1)  */
        seq_t f1 = (2 * i - 3);
        seq_t f2 = (2 * i - 2);
        seq_t den = (i - 1);
        P = (P * f1) * f2 / den;   /* exact division */
    }

    /* a(n) = (2*n-1)/n * P^2
       We avoid an intermediate overflow by dividing early:
          a = P * (P / n) * (2*n-1)     */
    seq_t P_div_n = P / n;                 /* exact division, P divisible by n */
    seq_t tmp     = P_div_n * (2 * n - 1);
    seq_t an      = tmp * P;
    return an;
}
