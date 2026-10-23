/*
 * A040213  Continued fraction for sqrt(229).
 * https://oeis.org/A040213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction expansion of sqrt(229)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sqrt_floor.h"

/* Computes the n-th term of the sequence A040213. */
seq_t a(idx_t n)
{
    /* Singleton state for the continued‑fraction data. */
    static bool initialized = false;
    static seq_t a0;              /* a(0) */
    static seq_t *period = NULL;  /* array of period terms a(1),a(2),… */
    static size_t period_len = 0;

    if (!initialized) {
        /* ----------------------------------------------------------------
         *  Compute the continued‑fraction expansion of sqrt(229)
         *  only once and cache it for subsequent calls.
         * --------------------------------------------------------------- */
        const seq_t D = (seq_t)229;   /* radicand */
        a0 = sqrt_floor(D);            /* floor(sqrt(229)) = 15 */

        seq_t m = 0;                  /* initial m */
        seq_t d = 1;                  /* initial d */
        seq_t a_val = a0;             /* a0 */

        /* Build the period array dynamically. */
        size_t cap = 4;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;

        /* Iterative continued‑fraction algorithm.  All terms after
         * a0 are stored into the period array.  The period ends
         * when a_val == 2*a0 (the first occurrence of the term 2*a0). */
        while (1) {
            /* Next step of the algorithm. */
            m = d * a_val - m;
            seq_t diff = D - m * m;     /* (N – m²) */
            d = diff / d;
            a_val = (a0 + m) / d;       /* new a */

            /* Store the term. */
            if (period_len == cap) {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_val;

            if (a_val == a0 * (seq_t)2)
                break;                 /* period completed */
        }

        initialized = true;
    }

    if (n == 0)
        return a0;                     /* first term */

    /* Index into the periodic part (a(1) corresponds to period[0]). */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
