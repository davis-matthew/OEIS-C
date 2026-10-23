/*
 * A042182  Numerators of continued fraction convergents to sqrt(616).
 * https://oeis.org/A042182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent of the continued
 *            fraction expansion of sqrt(616). This sequence is OEIS A042182
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   init_period
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/init_period.h"

/* The function is intentionally minimal and uses only the
   standard integer types provided by the build system. */

seq_t a(idx_t n)
{
    /* Static storage for the period of sqrt(616). The period length
       is always 10 for this D. */
    static idx_t period[32];
    static idx_t period_len = 0;
    static int  initialized = 0;

    if (!initialized) {
        init_period(period, &period_len);
        initialized = 1;
    }

    /* Recurrence for convergents: p_{-2}=0, p_{-1}=1.
       p_k = a_k * p_{k-1} + p_{k-2}. */
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t a_k;
        if (i == 0) {
            a_k = 24;                    /* a0 */
        } else {
            a_k = period[(i - 1) % period_len];
        }
        seq_t p = (seq_t)a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;                        /* p_n */
}
