/*
 * A213481  Number of triples (w,x,y) with all terms in {0,...,n} and |w-x| +
 *          |x-y| <= w+x+y.
 * https://oeis.org/A213481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (23*n^3 + 66*n^2 + 64*n + 24 - (3*n+6)*(n % 2)) / 24
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_mod2 = (seq_t)(n & 1ULL);          /* n modulo 2 */
    seq_t term1 = (seq_t)23 * nn * nn * nn;    /* 23 * n^3 */
    seq_t term2 = (seq_t)66 * nn * nn;         /* 66 * n^2 */
    seq_t term3 = (seq_t)64 * nn;              /* 64 * n   */
    seq_t term4 = (seq_t)24;                   /* constant 24 */
    seq_t parity = (seq_t)(3 * nn + 6) * n_mod2;
    seq_t num = term1 + term2 + term3 + term4 - parity;
    return num / (seq_t)24;
}
