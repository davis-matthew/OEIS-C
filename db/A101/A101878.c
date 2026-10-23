/*
 * A101878  Expansion of e.g.f. -LambertW(LambertW(-x))/x.
 * https://oeis.org/A101878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k+1)^k * (n+1)^(n-k-1) * C(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow_local.h"

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    seq_t sum       = (seq_t)0;
    seq_t C         = (seq_t)1;
    seq_t base_n    = (seq_t)(n + 1U);

    /* n+1^(n-1) for the term k = 0 (used in loop) and later for k = n */
    seq_t pow_n_term = seq_pow_local(base_n, n - 1U);

    for (idx_t k = 0U; k < n; ++k)
    {
        /* (k+1)^k */
        seq_t pow_k = seq_pow_local((seq_t)(k + 1U), k);

        /* term for current k */
        seq_t term = C * pow_k * pow_n_term;
        sum += term;

        /* update binomial coefficient C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        seq_t num = (seq_t)(n - k);
        seq_t den = (seq_t)(k + 1U);
        C = (C * num) / den;

        /* prepare pow_n_term for next iteration: divide by base_n */
        pow_n_term = pow_n_term / base_n;
    }

    /* term for k = n : (n+1)^(n-1) */
    sum += seq_pow_local(base_n, n - 1U);

    return sum;
}
