/*
 * A058086  a(n) = A058057(n,n-6).
 * https://oeis.org/A058086
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=6..n} (-1)^k * (n-k)! * binomial(k,6) *
 *            binomial(2*n-k,k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   binom_general
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_general.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;                /* sequence is defined only for n ≥ 6 */

    /* factorial part: (n-6)! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n - 6; ++i)
        fact = fact * (seq_t)i;

    /* binomial(k,6) starts at k = 6 -> C(6,6) = 1 */
    seq_t bin_k6 = 1;

    /* binomial(2n-k,k) starts at k = 6 */
    seq_t bin_2nk_k = binom_general(2 * n - 6, 6);

    seq_t sum = 0;

    for (idx_t k = 6; k <= n; ++k)
    {
        seq_t term = fact;
        term = term * bin_k6;
        term = term * bin_2nk_k;

        /* add or subtract based on parity of k */
        if (k & 1)
            sum -= term;
        else
            sum += term;

        /* update for next k, only if we are not at the last iteration */
        if (k < n)
        {
            /* (n-k)!   -> (n-(k+1))! = (n-k)! / (n-k) */
            fact = fact / (seq_t)(n - k);

            /* C(k+1,6) = C(k,6) * (k+1) / (k-5) */
            bin_k6 = bin_k6 * (seq_t)(k + 1);
            bin_k6 = bin_k6 / (seq_t)(k - 5);

            /* C(2n-(k+1),k+1) = C(2n-k,k) * (2n-2k)*(2n-2k-1) / ((k+1)*(2n-k)) */
            seq_t num = (seq_t)(2 * n - 2 * k) * (seq_t)(2 * n - 2 * k - 1);
            seq_t den = (seq_t)(k + 1) * (seq_t)(2 * n - k);
            bin_2nk_k = (bin_2nk_k * num) / den;
        }
    }

    return sum;
}
