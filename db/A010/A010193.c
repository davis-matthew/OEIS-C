/*
 * A010193  Continued fraction for sqrt(134).
 * https://oeis.org/A010193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of sqrt(134). */
/* a(0) = floor(sqrt(134)) = 11.  For n > 0, a(n) is the (n‑1)th term of the repeating block
   obtained from the simple continued fraction of sqrt(134). The period ends when
   the term equals 2 * a(0). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* period terms stored after first call */
    static seq_t period[14];
    static idx_t period_len = 0;
    static unsigned initialized = 0;

    if (!initialized) {
        /* compute the period of the continued fraction for sqrt(134) */
        const idx_t S = 134;
        seq_t fs = floor_sqrt((seq_t)S);   /* floor(sqrt(134)) == 11 */
        idx_t a0 = (idx_t)fs;              /* a0 = 11 */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        for (idx_t i = 0; ; ++i) {
            /* next values in continued fraction algorithm */
            idx_t old_d = d;
            m = d * a - m;
            d = (S - m * m) / old_d;
            a = (a0 + m) / d;

            period[i] = a;
            if (a == 2 * a0) {             /* period terminates */
                period_len = i + 1;
                break;
            }
        }

        initialized = 1;
    }

    if (n == 0)                 /* a(0) */
        return (seq_t)((idx_t)floor_sqrt((seq_t)134));

    idx_t rem = (n - 1) % period_len;
    return period[rem];
}
