/*
 * A040396  Continued fraction for sqrt(417).
 * https://oeis.org/A040396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(417)
   a(0) = floor(sqrt(417))
   a(1..∞) = the periodic part of the continued fraction of sqrt(417)   */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return the continued‑fraction term a(n) for sqrt(417). */
seq_t a(idx_t n)
{
    /* --------- Cached period data --------- */
    static seq_t *period = NULL;     /* array of periodic terms          */
    static idx_t period_len = 0;     /* length of that period           */
    static seq_t a0 = 0;             /* floor(sqrt(417))                */
    static bool initialized = false;

    /* ---- initialisation on first call ---- */
    if (!initialized) {
        /* --- compute floor(sqrt(417)) without floating point --- */
        const idx_t D = 417;
        a0 = 0;
        for (seq_t i = 0;; ++i) {
            /* stop when (i+1)^2 > 417 */
            if ((i + 1) * (i + 1) > D) {
                a0 = i;
                break;
            }
        }

        /* --- generate the periodic part --- */
        size_t cap = 32;               /* start with a modest capacity */
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t len = 0;

        seq_t m = 0;                  /* m0 = 0 */
        seq_t d = 1;                  /* d0 = 1 */
        seq_t a = a0;                 /* a0 = floor(sqrt(D)) */

        do {
            /* usual recurrence for continued fractions of sqrt(D) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            if (len == cap) {
                cap <<= 1;            /* double capacity when needed */
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[len++] = a;
        } while (a != (seq_t)(2 * a0));   /* period ends when a == 2*a0 */

        period_len = (idx_t)len;
        initialized = true;
    }

    /* -------- answer the query ---------- */
    if (n == 0) {
        return a0;
    } else {
        /* convert loop index to size_t safely (n>0 here) */
        size_t idx = (size_t)((n - 1) % period_len);
        return period[idx];
    }
}
