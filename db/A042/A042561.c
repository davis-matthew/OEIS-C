/*
 * A042561  Denominators of continued fraction convergents to sqrt(809).
 * https://oeis.org/A042561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of n-th convergent to sqrt(809)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first term */
    if (n == 0) return (seq_t)1;

    /* constants for D = 809 */
    const idx_t D = 809;
    static idx_t a0 = 0;                /* floor(sqrt(809)) */
    static idx_t *period = NULL;        /* period sequence a1..aL */
    static idx_t period_len = 0;        /* length L of the period */
    static bool initialized = false;

    if (!initialized) {
        /* compute integer floor of sqrt(809) */
        idx_t t = 0;
        while ((t + 1) * (t + 1) <= D) ++t;
        a0 = t;           /* 28 */

        /* compute the period of the continued fraction */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        /* upper bound for period length (≈ 2*sqrt(D)) */
        const idx_t maxlen = 2000;
        period = (idx_t *)malloc(maxlen * sizeof(idx_t));
        if (!period) return (seq_t)0; /* allocation failure – should not happen */

        idx_t idx = 0;
        while (true) {
            /* continued‑fraction recurrence for sqrt(D) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            period[idx++] = a;
            if (a == 2 * a0) break;   /* period ends when a == 2*a0 */
        }
        period_len = idx;
        initialized = true;
    }

    /* recurrence to compute q_n (denominator) */
    seq_t p_pp = 0;   /* p_{k-2} */
    seq_t p_p  = 1;   /* p_{k-1} */
    seq_t q_pp = 1;   /* q_{k-2} */
    seq_t q_p  = 0;   /* q_{k-1} */
    seq_t a_k;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            a_k = (seq_t)a0;
        } else {
            a_k = (seq_t)period[(k - 1) % period_len];
        }
        seq_t p_c = a_k * p_p + p_pp;
        seq_t q_c = a_k * q_p + q_pp;
        p_pp = p_p;  p_p = p_c;
        q_pp = q_p;  q_p = q_c;
    }
    return q_p;
}
