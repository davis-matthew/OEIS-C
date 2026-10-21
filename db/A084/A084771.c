/*
 * A084771  Coefficients of expansion of 1/sqrt(1 - 10*x + 9*x^2); also, a(n)
 *          is the central coefficient of (1 + 5*x + 4*x^2)^n.
 * https://oeis.org/A084771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} 4^k * C(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a multiplicative recurrence for C(n,k) to achieve O(n) time. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;            /* running total */
    seq_t C   = 1;            /* current binomial C(n,k) */
    seq_t pow4 = 1;           /* 4^k */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k)^2 * 4^k */
        seq_t term = C * C * pow4;
        sum += term;

        /* Prepare next values (except at the last iteration) */
        if (k == n)
            break;

        /* C(n,k+1) = C * (n - k) / (k + 1) */
        C = C * (seq_t)(n - k);
        C = C / (seq_t)(k + 1);

        /* pow4 = 4^(k+1) */
        pow4 = pow4 * 4;
    }

    return sum;
}
