/*
 * A027277  a(n) = Sum_{k=0..n} binomial(2*k,k)*binomial(2*n-k,k).
 * https://oeis.org/A027277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k)*binomial(2*n-k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t B = 1;    /* binomial(2*k,k)   */
    seq_t C = 1;    /* binomial(2*n-k,k) */
    for (idx_t k = 0; k <= n; ++k) {
        result += B * C;

        if (k == n)
            break;

        /* Update B to binomial(2*(k+1),k+1) */
        seq_t num_B = (seq_t)2 * (seq_t)(k + 1) * (seq_t)(2 * (k + 1) - 1);
        seq_t den_B = (seq_t)(k + 1) * (seq_t)(k + 1);
        B = B * num_B / den_B;

        /* Update C to binomial(2*n-(k+1),k+1) */
        seq_t num_C = (seq_t)(2 * n - 2 * k) * (seq_t)(2 * n - 2 * k - 1);
        seq_t den_C = (seq_t)(k + 1) * (seq_t)(2 * n - k);
        C = C * num_C / den_C;
    }
    return result;
}
