/*
 * A052885  E.g.f. A(x) is inverse to F(x) = x*exp(-x)/(1+x).
 * https://oeis.org/A052885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(n) = (n‑1)! * Σ_{t=0}^{n‑1} n^t * binomial(n, t+1) /
 *            t! (n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A052885: number of derangements with a particular e.g.f. inverse. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *fact = NULL;      /* factorials: fact[i] = i!   */
static size_t fact_len = 0;     /* number of factorials allocated */

/* Allocate factorials up to *n* inclusive.  */ 
static void ensure_fact(idx_t n)
{
    if (fact_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        fact = (seq_t *)realloc(fact, new_len * sizeof(seq_t));
        if (!fact) exit(1);          /* simple error handling */
        for (size_t i = fact_len; i <= n; ++i) {
            if (i == 0)
                fact[i] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = new_len;
    }
}

/* Sequence a(n) for A052885. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;

    ensure_fact(n - 1u);

    seq_t sum     = (seq_t)0;
    seq_t pow     = (seq_t)1;          /* n^t */
    seq_t fac_div = fact[n - 1u];      /* (n‑1)! / t!  for current t */

    /* t goes from 0 to n‑1 */
    for (idx_t t = 0; t < n; ++t) {
        seq_t bin  = binomial(n, t + 1u);          /* binomial(n, t+1)   */
        seq_t term = bin * pow;                    /* bin * n^t          */
        term = term * fac_div;                     /* * ((n‑1)!/t!)      */
        sum += term;                               /* accumulate          */

        /* update for next iteration */
        pow = pow * (seq_t)n;                      /* n^(t+1)            */
        if (t + 1u < n)                           /* fac_div for t+1    */
            fac_div = fac_div / (idx_t)(t + 1u);
    }
    return sum;
}
