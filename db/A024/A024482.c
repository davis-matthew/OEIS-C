/*
 * A024482  a(n) = (1/2)*(binomial(2n, n) - binomial(2n-2, n-1)).
 * https://oeis.org/A024482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-2)*Catalan(n-1)/2 (Catalan(k)=binom(2k,k)/(k+1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 */

    /* Compute Catalan(n-1) iteratively: C(0)=1,
       C(i+1)=C(i)*(4*i+2)/(i+2) */
    seq_t catalan = 1;                    /* C0 */
    for (idx_t i = 0; i <= n - 2; ++i) {
        seq_t mult = (seq_t)(4 * i + 2);
        catalan = (catalan * mult) / (seq_t)(i + 2);
    }

    seq_t factor = (seq_t)(3 * n - 2);
    if (factor & 1) {                     /* factor odd → catalan even */
        return (catalan / 2) * factor;
    } else {
        return catalan * (factor / 2);
    }
}
