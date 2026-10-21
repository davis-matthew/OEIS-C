/*
 * A055795  a(n) = binomial(n,4) + binomial(n,2).
 * https://oeis.org/A055795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,4) + binomial(n,2)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 288, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,4) is defined only for n >= 4; 
       binomial(n,2) is defined for n >= 2.
       For smaller n we avoid underflow by guarding early. */
    if (n < 2)
        return 0;                   /* both binomials are zero */

    seq_t m = (seq_t)n;
    seq_t binom2 = m * (m - 1) / (seq_t)2;   /* n*(n-1)/2 */

    if (n < 4)
        return binom2;              /* binom(n,4) is zero for n < 4 */

    seq_t binom4 = m * (m - 1) * (m - 2) * (m - 3) / (seq_t)24; /* n*(n-1)*(n-2)*(n-3)/24 */

    return binom2 + binom4;
}
