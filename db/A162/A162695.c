/*
 * A162695  E.g.f. satisfies A(x) = exp( x*A(x) * exp(x*A(x)) ).
 * https://oeis.org/A162695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (n+1)^(k-1) * k^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t used by the headers */

static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    seq_t base = (seq_t)(n + 1);          /* (n+1) */
    seq_t pow_nplus = 1;                  /* (n+1)^(k-1) for current k */
    seq_t binom = 1;                      /* binomial(n,0) initially */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial(n,k) from binomial(n,k-1) */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / k;                /* exact division */

        /* k^(n-k) */
        seq_t pow_k = pow_uint((seq_t)k, n - k);

        /* term = binom * (n+1)^(k-1) * k^(n-k) */
        seq_t term = binom * pow_nplus;
        term *= pow_k;

        sum += term;

        /* Prepare (n+1)^k for next iteration */
        pow_nplus *= base;
    }

    return sum;
}
