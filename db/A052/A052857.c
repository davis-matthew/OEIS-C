/*
 * A052857  A simple grammar. a(n)=n*A052873(n-1).
 * https://oeis.org/A052857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n-2} C(n-2,k) * n^k / (k+1)! (derived from
 *            the hypergeometric definition a(n) = n!*1F1(-n+2;2;-n) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t sum  = fact;   /* term for k=1 */
    seq_t term = fact;   /* current term (k) */
    for (idx_t k = 1; k <= n - 2; ++k) {
        /* compute next term using: term' = term * n * (n-1-k) / (k*(k+1)) */
        term *= (seq_t)n;
        term *= (seq_t)(n - 1 - k);
        idx_t denom = k * (k + 1);
        term /= (seq_t)denom;
        sum += term;
    }
    return sum;
}
