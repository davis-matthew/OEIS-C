/*
 * A024421  a(n) = n!*(1/C(n,0) - 1/C(n,1) - ... - 1/C(n,[ n/2 ])).
 * https://oeis.org/A024421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1/C(n,0) - 1/C(n,1) - … - 1/C(n, floor(n/2)))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    seq_t ans = fact_n;
    idx_t m = n / 2;
    if (m == 0) return ans;     /* n = 0 or 1 */

    seq_t fact_k = 1;   /* k! for current k */
    seq_t fact_nk = fact_n; /* (n-k)! for current k */

    for (idx_t k = 1; k <= m; ++k) {
        fact_k *= (seq_t)k;                    /* k!  */
        fact_nk /= (seq_t)(n - k + 1);         /* (n-k)!  */
        seq_t term = fact_k * fact_nk;         /* k!*(n-k)! */
        ans -= term;
    }
    return ans;
}
