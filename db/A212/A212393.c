/*
 * A212393  Expansion of
 *          (1-4*x+7*x^2-5*x^3+4*x^4-6*x^5+21*x^6+18*x^7-5*x^8)/(1-x)^5.
 * https://oeis.org/A212393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(31*n^3-369*n^2+1454*n-1560)/24 for n>3 a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 120, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* n > 3: use closed‑form formula */
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;            /* n^2 */
    seq_t N3 = N2 * N;           /* n^3 */
    seq_t term = (31 * N3)       /* 31*n^3   */
               - (369 * N2)      /* -369*n^2  */
               + (1454 * N)      /* +1454*n   */
               - 1560;           /* -1560     */
    seq_t result = ((N - 3) * term) / 24;
    return result;
}
