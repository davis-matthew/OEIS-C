/*
 * A041538  Numerators of continued fraction convergents to sqrt(286).
 * https://oeis.org/A041538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for the numerators of the convergents to sqrt(286).
 * a(0) = 16
 * Let c[0] = 16 and for n ≥ 1 let c[n-1] be the (n‑1)th term in the period
 * [1,10,3,3,2,3,3,10,1,32].  Then for n ≥ 1  
 *    a(n) = c[n-1] * a(n-1) + a(n-2).
 * This recurrence produces OEIS A041538.
 */
#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    /* Periodic part of the continued–fraction coefficients (after a(0)). */
    static const int period_vals[10] = {1, 10, 3, 3, 2, 3, 3, 10, 1, 32};

    seq_t p_prevprev = 0;   /* a(-2) */
    seq_t p_prev     = 1;   /* a(-1) */
    seq_t p_curr     = 0;   /* a(n) */

    for (idx_t k = 0; k <= n; ++k) {
        int c_k;
        if (k == 0) {
            c_k = 16;                      /* the initial integer part */
        } else {
            /* Repeat the 10‑term period for k ≥ 1. */
            c_k = period_vals[(size_t)(k - 1) % 10];
        }

        /* a(k) = c_k * a(k-1) + a(k-2) */
        p_curr = (seq_t)c_k * p_prev + p_prevprev;

        /* shift the last two values for next iteration */
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_curr;
}
