/*
 * A086406  Main diagonal of number array A086404.
 * https://oeis.org/A086406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} binom(n,k) * n^(n-k) * 3^{floor(k/2)}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the closed form in OEIS. */
seq_t a(idx_t n)
{
    /* If n is zero, return the first term */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;

    /* Compute n^n iteratively (may overflow for large n, which is
       acceptable because tests only check values that fit in seq_t). */
    seq_t pow_n = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow_n *= (seq_t)n;
    }

    /* binomial coefficient C(n,0) starts at 1 */
    seq_t binom = 1;

    /* 3^floor(k/2) for k=0 initially equals 1 */
    seq_t pow3 = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = C(n,k) * n^(n-k) * 3^{floor(k/2)} */
        seq_t term = binom;
        term *= pow_n;
        term *= pow3;
        sum += term;

        /* Prepare values for the next k */
        if (k == n)
            break;

        /* Update binomial coefficient: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        binom *= (seq_t)(n - k);
        binom /= (seq_t)(k + 1);

        /* Update n^(n-k-1) = n^(n-k) / n */
        pow_n /= (seq_t)n;

        /* Update 3^floor((k+1)/2) after processing an odd k */
        if (k & 1)
            pow3 *= (seq_t)3;
    }

    return sum;
}
