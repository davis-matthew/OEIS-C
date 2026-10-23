/*
 * A040686  Continued fraction for sqrt(713).
 * https://oeis.org/A040686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = nth term of the continued fraction expansion of sqrt(713)
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *period = NULL;
static size_t period_len = 0;
static seq_t a0 = 0;
static int period_init = 0;

/* Compute the periodic part of the continued fraction for sqrt(713) once   */
static void init_period(void) {
    if (period_init) {
        return;
    }

    const seq_t D = (seq_t)713;
    a0 = floor_sqrt(D);

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    size_t cap = 8;
    period = (seq_t *)malloc(cap * sizeof(seq_t));
    period_len = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;          /* floor division */

        if (period_len == cap) {
            cap *= 2;
            period = (seq_t *)realloc(period, cap * sizeof(seq_t));
        }
        period[period_len++] = a;

        if (a == 2 * a0) {
            break;                  /* end of period for sqrt(713) */
        }
    }

    period_init = 1;
}

/* Return the nth term of the continued fraction for sqrt(713)              */
seq_t a(idx_t n) {
    init_period();

    if (n == 0) {
        return a0;
    }

    idx_t pos = (n - 1) % (idx_t)period_len;
    return period[(size_t)pos];
}
