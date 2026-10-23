/*
 * A040951  Continued fraction for sqrt(983).
 * https://oeis.org/A040951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction coefficients of sqrt(983); periodic sequence.
 *            a(0) = floor(sqrt(983)), a(n) for n>0 follows the periodic part
 *            that repeats with period length L, computed via the standard
 *            continued‑fraction expansion algorithm
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return the n-th term of the sequence A040951.  */
seq_t a(idx_t n)
{
    /* The radicand (always the same value). */
    const idx_t D = 983;

    /* State for the periodic part.  Initialized once. */
    static bool initialized = false;
    static seq_t a0 = 0;            /* floor(sqrt(D)) */
    static seq_t *period = NULL;   /* array of periodic coefficients */
    static size_t period_len = 0;   /* length of period */

    if (!initialized) {
        /* Compute a0. */
        a0 = floor_sqrt((seq_t)D);

        /* Compute the periodic part.  We can allocate a small array
           and grow it if necessary; the period of sqrt(983) is small.  */
        size_t cap = 8;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period) return 0;     /* out of memory – unlikely */

        seq_t m = 0, d = 1, a = a0;
        do {
            /* Next state. */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            /* Store the coefficient. */
            if (period_len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(seq_t));
                if (!tmp) { free(period); return 0; }
                period = tmp;
            }
            period[period_len++] = a;
        } while (a != (a0 << 1));   /* period ends when a == 2*a0 */

        initialized = true;
    }

    if (n == 0) {
        return a0;
    } else {
        /* Sequence repeats after a0 with the periodic part. */
        size_t idx = (size_t)(n - 1) % period_len;
        return period[idx];
    }
}
