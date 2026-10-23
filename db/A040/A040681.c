/*
 * A040681  Continued fraction for sqrt(708).
 * https://oeis.org/A040681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = coefficient of the continued fraction expansion of
 *            sqrt(708). The sequence starts at a(0)=26 and is periodic with
 *            period length 12
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static seq_t a0 = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Compute the continued‑fraction coefficients of sqrt(708). */
        const seq_t D = (seq_t)708;
        a0 = floor_sqrt(D);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_k = a0;

        size_t cap = 4;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;

        while (true) {
            /* next triple (m, d, a) */
            m = d * a_k - m;
            d = (D - m * m) / d;
            a_k = (a0 + m) / d;

            /* store the coefficient a_k (excluding the initial a0) */
            if (period_len == cap) {
                cap <<= 1;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_k;

            if (a_k == 2 * a0)  /* end of period */
                break;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* periodic part: indices 0..period_len-1 correspond to a(1)..a(period_len) */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
