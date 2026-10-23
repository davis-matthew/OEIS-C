/*
 * A040711  Continued fraction for sqrt(739).
 * https://oeis.org/A040711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for √739.  a(0) = ⌊√739⌋ = 27, 
   subsequent terms repeat with the period computed on the first call. */

#include <stdlib.h>

static const idx_t D = 739ULL;               /* the radicand */
static const idx_t a0 = 27ULL;               /* floor(sqrt(D)) */
static seq_t *period = NULL;                /* terms a(1)..a(period_len) */
static size_t period_len = 0;                /* number of periodic terms */
static size_t period_cap = 0;                /* allocated capacity for period */

seq_t a(idx_t n)
{
    /* Initialise the periodic part on the first request */
    if (period_len == 0) {
        /* Prepare storage */
        period_cap = 64;
        period = (seq_t *)malloc(period_cap * sizeof(seq_t));
        if (!period) return (seq_t)0;      /* allocation failure */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        /* Generate continued‑fraction terms until d == 1 (end of period) */
        do {
            /* next triple (m, d, a) */
            m = d * a - m;
            d = (idx_t)(D - m * m) / d;
            a = (idx_t)(a0 + m) / d;

            /* Store a in the period array */
            if (period_len == period_cap) {
                period_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, period_cap * sizeof(seq_t));
                if (!tmp) {
                    free(period);
                    period = NULL;
                    period_len = 0;
                    return (seq_t)0;
                }
                period = tmp;
            }
            period[period_len++] = (seq_t)a;
        } while (d != 1);   /* period ends when d becomes 1 */
    }

    /* Answer the request */
    if (n == 0)
        return (seq_t)a0;

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
