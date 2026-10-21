/*
 * A054132  T(2n+1,n), array T as in A054126.
 * https://oeis.org/A054132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + Sum_{m=0..n+1} binomial(3*n+1, m)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;

    /* partial sum of binomial coefficients up to m = n+1 */
    seq_t sum   = 1;       /* C(3n+1, 0) */
    seq_t binom = 1;       /* current binomial value */
    idx_t   N    = 3 * n + 1;   /* the top of the binomial */

    for (idx_t m = 1; m <= n + 1; ++m)
    {
        binom = binom * (N - m + 1) / m;
        sum  += binom;
    }

    return sum + pow2;
}
