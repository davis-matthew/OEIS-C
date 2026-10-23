/*
 * A041770  Numerators of continued fraction convergents to sqrt(406).
 * https://oeis.org/A041770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=20; for n>0: a(n) = a(n-1)*c_n + a(n-2) where the
 *            coefficient sequence (c_n) is periodic with period 14: [6, 1, 2,
 *            4, 7, 1, 4, 1, 7, 4, 2, 1, 6, 40]
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the first term directly */
    if (n == 0)
        return (seq_t)20;

    /* Periodic coefficients after the first term */
    static const unsigned long long period[] = {
        6ULL, 1ULL, 2ULL, 4ULL, 7ULL, 1ULL,
        4ULL, 1ULL, 7ULL, 4ULL, 2ULL, 1ULL,
        6ULL, 40ULL
    };
    const size_t period_len = sizeof(period) / sizeof(period[0]);

    /* Initial values for the recurrence: h_{-2}=0, h_{-1}=1 */
    seq_t h_prev2 = 0;      /* h_{i-2} */
    seq_t h_prev1 = 1;      /* h_{i-1} */

    /* Compute h_0 using a_0 = 20 */
    seq_t h_curr = ((seq_t)20) * h_prev1 + h_prev2;
    h_prev2 = h_prev1;
    h_prev1 = h_curr;

    /* Iterate up to the requested index n */
    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)((i - 1) % period_len);
        seq_t coeff = (seq_t)period[idx];
        h_curr = coeff * h_prev1 + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }

    return h_prev1;
}
