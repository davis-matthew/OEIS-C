/*
 * A182525  a(n) = n! * Sum_{k=0..n} binomial(2*n, 2*k) / binomial(n,k).
 * https://oeis.org/A182525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..n} binomial(2*n, 2*k) / binomial(n, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* factorial array fact[0..n] */
    size_t sz = (size_t)n + 1;
    seq_t *fact = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fact) return 0; /* allocation failure */

    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t sum = 0;
    seq_t binom = 1; /* binom(2*n, 0) is 1 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* update binom to binom(2*n, 2*k) */
            seq_t t = binom * (seq_t)(2 * n - 2 * k + 2);
            t = t / (seq_t)(2 * k - 1);
            t = t * (seq_t)(2 * n - 2 * k + 1);
            binom = t / (seq_t)(2 * k);
        }

        seq_t term = binom;
        term = term * fact[k];
        term = term * fact[n - k];
        sum += term;
    }

    free(fact);
    return sum;
}
