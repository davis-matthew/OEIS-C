/*
 * A042262  Numerators of continued fraction convergents to sqrt(657).
 * https://oeis.org/A042262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 24, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the numerator p_n of the n‑th convergent of the continued
   fraction expansion of sqrt(657).
   The partial denominators are produced by the standard
   recurrence:
     m_0 = 0, d_0 = 1, a_0 = floor(sqrt(657))
     m_k = d_{k-1}*a_{k-1} - m_{k-1}
     d_k = (657 - m_k^2) / d_{k-1}
     a_k = (a_0 + m_k) / d_k
   a_k repeats when d_k == 1 and a_k == 2*a_0.
   The numerators satisfy
     p_{-2} = 0, p_{-1} = 1
     p_k   = a_k * p_{k-1} + p_{k-2}   for k >= 0   (a_0 is as above).
   a(0) = 25, a(1) = 26, ...

   This implementation builds the periodic part once and uses
   integer arithmetic only. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute a0 = floor(sqrt(657)) once per call, but we can memoize
       it if desired.  For clarity we calculate it directly. */
    const seq_t D  = (seq_t)657;
    const seq_t a0 = floor_sqrt(D);

    if (n == 0)
        return a0;

    /* --- compute the periodic part of the continued fraction once --- */
    static seq_t *period = NULL;
    static idx_t period_len = 0;
    static idx_t period_cap = 0;

    if (period == NULL) {
        period_cap = 64;
        period = (seq_t *)malloc(period_cap * sizeof(seq_t));
        if (period == NULL)
            abort();                /* allocation failure */

        /* Build period: a(1), a(2), ... until repetition. */
        seq_t m = 0, d = 1, a = a0;
        seq_t two_a0 = a0 * 2;

        do {
            seq_t m1 = d * a - m;
            seq_t d1 = (D - m1 * m1) / d;
            seq_t a1 = (a0 + m1) / d1;

            /* Append a1 */
            if (period_len == period_cap) {
                period_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, period_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    free(period);
                    abort();
                }
                period = tmp;
            }
            period[period_len++] = a1;

            /* update for next iteration */
            m = m1;
            d = d1;
            a = a1;
        } while (!(d == 1 && a == two_a0));   /* repeat detection */
    }

    /* --- compute numerator p_n via recurrence --- */
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p_n   = 0;   /* p_k */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? a0 : period[(i - 1) % period_len];
        p_n = ai * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
