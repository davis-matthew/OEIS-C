/*
 * A088695  E.g.f. satisfies A(x) = f(x*A(x)), where f(x) = exp(x+x^2).
 * https://oeis.org/A088695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=ceil(n/2)}^n binomial(k,n-k)*(n+1)^(k-1)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sum efficiently using only
 * integer arithmetic and avoids large factorial divisions on each step. */
#include <stdlib.h>

static seq_t *fact = NULL;      /* factorials 0! ... n! */
static idx_t fact_len = 0;      /* number of computed factorials */

/* Ensure that factorials up to n are available */
static void ensure_fact(idx_t n)
{
    if (fact_len <= n) {
        /* extend factorial array */
        idx_t new_len = n + 1;
        fact = (seq_t *)realloc(fact, (size_t)new_len * sizeof(seq_t));
        if (fact_len == 0) {
            fact[0] = (seq_t)1;
            for (idx_t i = 1; i <= n; ++i)
                fact[i] = fact[i-1] * (seq_t)i;
        } else {
            for (idx_t i = fact_len; i <= n; ++i)
                fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_fact(n);                     /* factorials up to n */

    idx_t k_start = (n + 1) >> 1;        /* ceil(n/2) */

    /* r_start = n! / ((n-k_start)! * (2*k_start-n)!) */
    seq_t r_start = 1;
    for (idx_t i = n - k_start + 1; i <= n; ++i) r_start *= (seq_t)i;
    idx_t denom_idx = 2 * k_start - n;   /* 0 or 1 */
    r_start /= fact[denom_idx];

    /* power part: pow_start = (n+1)^(k_start-1) */
    seq_t pow_val = 1;
    seq_t np1 = (seq_t)(n + 1);
    for (idx_t i = 1; i < k_start; ++i) pow_val *= np1;

    /* first term and running sum */
    seq_t term = r_start * pow_val;
    seq_t sum = term;

    /* iterate for k = k_start+1 .. n */
    for (idx_t k = k_start + 1; k <= n; ++k) {
        idx_t k_prev = k - 1;
        seq_t mult1 = (seq_t)(n - k_prev);
        idx_t t1 = 2 * k_prev - n + 2;
        idx_t t2 = 2 * k_prev - n + 1;
        seq_t mult2 = (seq_t)(t1 * t2);

        term = term * mult1;        /* term *= (n - k_prev) */
        term = term * np1;          /* term *= (n+1)    */
        term = term / mult2;        /* divide by (t1*t2) */

        sum += term;
    }

    return sum;
}
