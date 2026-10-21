/*
 * A050148  a(n) = T(n,n-2), array T as in A050143.
 * https://oeis.org/A050148
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1..n} binomial(n,s)*binomial(s+n-4,n-3) * for n ≥
 *            3, with a(2) = 1. This implementation evaluates the sum *
 *            iteratively using the multiplicative formulas for the binomial *
 *            coefficients, so it runs in O(n) time and requires only a few *
 *            additional variables. All arithmetic is perfor...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; below that the result is
       undefined – return 0. */
    if (n < 2) return 0;

    /* Base case */
    if (n == 2) return (seq_t)1;

    seq_t sum = 0;

    /* binomial(n,1) and binomial(n-3,n-3) for s = 1 */
    seq_t b1 = (seq_t)n;   /* binomial(n,1)      */
    seq_t b2 = 1;          /* binomial(n-3,n-3) */

    for (idx_t s = 1; s <= n; ++s) {
        /* Add the term binomial(n,s) * binomial(s+n-4, n-3) */
        sum += (seq_t)((unsigned __int128)b1 * b2);

        if (s == n) break;  /* nothing to update on the last iteration */

        /* Update b1 to binomial(n, s+1) */
        unsigned __int128 tmp1 = (unsigned __int128)b1 * (n - s);
        b1 = (seq_t)(tmp1 / (s + 1));

        /* Update b2 to binomial(s+1 + n - 4, n-3) */
        unsigned __int128 tmp2 = (unsigned __int128)b2 * (s + n - 3);
        b2 = (seq_t)(tmp2 / s);
    }

    return sum;
}
