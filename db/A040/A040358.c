/*
 * A040358  Continued fraction for sqrt(378).
 * https://oeis.org/A040358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√378⌋. For n > 0, a(n) is the (n‑1)th coefficient in the
 *            simple continued fraction expansion of √378; the period is 8,
 *            but this is computed algorithmically from the definition so no
 *            explicit table is hard‑coded
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Integer part of √378 */
    /* The cast works for both C (to an unsigned integer type) and C++ (to
       the arbitrary‑precision class's constructor). */
    const seq_t a0 = (seq_t)19;   /* floor(sqrt(378)) */

    /* On first call, generate the repeating part of the expansion. */
    static bool initialized = false;
    static seq_t *period = NULL;
    static size_t period_len = 0;

    if (!initialized) {
        /* Parameters for the continued fraction algorithm. */
        const seq_t D = (seq_t)378;          /* the number under the square root */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_coef = a0;                  /* will become a(1) on the first loop */

        /* Temporaries to hold the limit a_max = 2*a0 for period detection. */
        const seq_t a_max = a0 * (seq_t)2;

        /* Allocate space for the period terms; it will be reallocated if
           necessary (the period length for √378 is 8). */
        size_t cap = 8;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period) return (seq_t)0;        /* Out‑of‑memory guard */

        /* Generate period until the coefficient equals twice a0. */
        period_len = 0;
        while (1) {
            /* Compute the next coefficient in the expansion. */
            m = d * a_coef - m;
            d = (D - m * m) / d;
            a_coef = (a0 + m) / d;

            /* Store the coefficient (starting with a(1)). */
            if (period_len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp) { free(period); return (seq_t)0; }
                period = tmp;
            }
            period[period_len++] = a_coef;

            /* If the computed coefficient equals twice the integer part,
               the period has closed. */
            if (a_coef == a_max) {
                break;
            }
        }
        initialized = true;
    }

    /* Return the requested term. */
    if (n == 0) {
        return a0;
    } else {
        idx_t idx = (n - 1) % period_len;
        return period[idx];
    }
}
