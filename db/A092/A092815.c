/*
 * A092815  Schmidt's problem sum for r = 5.
 * https://oeis.org/A092815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum(k=0..n, binomial(n,k)^5 * binomial(n+k,k)^5 )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow5(seq_t x)
{
    return x * x * x * x * x;
}

seq_t a(idx_t n)
{
    idx_t k = 0;
    seq_t binom_nk = 1;       /* binomial(n, k)      */
    seq_t binom_npok = 1;     /* binomial(n+k, k)    */
    seq_t sum = 0;

    while (k <= n) {
        seq_t p1 = pow5(binom_nk);
        seq_t p2 = pow5(binom_npok);
        sum += p1 * p2;

        ++k;
        if (k > n) break;

        /* Update binomial coefficients for next k */
        binom_nk = binom_nk * (seq_t)(n - (k - 1)) / (seq_t)k;
        binom_npok = binom_npok * ((seq_t)n + (seq_t)k) / (seq_t)k;
    }

    return sum;
}
