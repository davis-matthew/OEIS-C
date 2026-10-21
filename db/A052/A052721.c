/*
 * A052721  Expansion of e.g.f. x*(1-2*x)*(1 - 2*x - sqrt(1-4*x))/2 - x^3.
 * https://oeis.org/A052721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(n-4)*(2*(n-3))!/(n-3)! (n>=5),
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    idx_t k = n - 3;          /* k >= 2 for n >= 5 */
    seq_t prod = 1;

    /* product (k+1)(k+2)…(2k) = (2k)! / k!                */
    for (idx_t i = 1; i <= k; ++i) {
        prod *= (seq_t)(k + i);
    }

    /* a(n) = 2 * n * (n-4) * prod                     */
    seq_t res = prod;
    res *= 2;
    res *= (seq_t)n;
    res *= (seq_t)(n - 4);
    return res;
}
