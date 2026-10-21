/*
 * A065920  Bessel polynomial {y_n}'(2).
 * https://oeis.org/A065920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n-1)^2 * a(n) = (2*n-1)*(2*n^2-2*n+1)*a(n-1) + n^2*a(n-2) a(0)
 *            = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_minus2 = (seq_t)0;   /* a(0) */
    seq_t a_minus1 = (seq_t)1;   /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)(2*i-1) * (seq_t)(2*i*i - 2*i + 1);
        term1 *= a_minus1;
        seq_t term2 = (seq_t)(i*i) * a_minus2;
        seq_t numerator = term1 + term2;
        seq_t denom = (seq_t)(i-1) * (seq_t)(i-1);
        a_n = numerator / denom;
        a_minus2 = a_minus1;
        a_minus1 = a_n;
    }
    return a_minus1;
}
#else
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_minus2 = (seq_t)0;   /* a(0) */
    seq_t a_minus1 = (seq_t)1;   /* a(1) */
    seq_t a_n = a_minus1;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 term1 =
            (unsigned __int128)(2*i-1) *
            (unsigned __int128)(2*i*i - 2*i + 1) *
            (unsigned __int128)a_minus1;
        unsigned __int128 term2 =
            (unsigned __int128)(i*i) *
            (unsigned __int128)a_minus2;
        unsigned __int128 numerator = term1 + term2;
        unsigned __int128 denom = (unsigned __int128)(i-1) * (unsigned __int128)(i-1);
        unsigned __int128 ai = numerator / denom;
        a_minus2 = a_minus1;
        a_minus1 = (seq_t)ai;
    }
    return a_minus1;
}
#endif
