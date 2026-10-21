/*
 * A054652  Number of acyclic orientations of the Hamming graph (K_2) x (K_n).
 * https://oeis.org/A054652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (n!/k!) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of acyclic orientations of the Hamming graph (K_2) × (K_n). */
seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* binomial coefficient C(n,0) = 1 */
    seq_t bin = 1;

    /* prod = n! / k!  (k = 0) */
    seq_t prod = fact_n;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = prod * bin = (n! / k!) * C(n,k) */
        seq_t term = prod * bin;
        sum += term;

        if (k == n)
            break;                     /* no further updates */

        /* update to next binomial C(n,k+1) */
        seq_t mul_num = (seq_t)(n - k);
        seq_t mul_den = (seq_t)(k + 1);
        bin = bin * mul_num / mul_den;

        /* update prod to n! / (k+1)! */
        prod /= mul_den;
    }

    /* a(n) = n! * sum */
    return fact_n * sum;
}
