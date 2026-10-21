/*
 * A052871  Expansion of e.g.f. -LambertW(x/(-1+x)).
 * https://oeis.org/A052871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (n!/k!)*binomial(n-1, k-1)*k^(k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 0 */
    if (n == 0) return 0;

    /* Compute n! once */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    seq_t sum = 0;
    seq_t binom = 1;          /* binomial(n-1,0) */
    seq_t fact_k = 1;         /* k! initially for k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* k^(k-1) using provided big_pow */
        seq_t powk = big_pow((seq_t)k, (seq_t)(k - 1));

        /* term = (n!/k!)*binom*k^(k-1) */
        seq_t term = fact_n / fact_k;   /* integer division, exact */
        term = term * binom;
        term = term * powk;

        sum += term;

        if (k < n) {
            /* update binomial coefficient for next k:
               C(n-1,k) = C(n-1,k-1) * (n-k) / k  */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
            /* update k! for next k: (k+1)! */
            fact_k *= (seq_t)(k + 1);
        }
    }

    return sum;
}
