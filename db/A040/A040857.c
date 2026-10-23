/*
 * A040857  Continued fraction for sqrt(887).
 * https://oeis.org/A040857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   the terms of the continued fraction expansion of √887. a(0) =
 *            ⌊√887⌋, and for n ≥ 1 the terms repeat with period 12
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The period for √887 is computed by the standard continued‑fraction
   algorithm and stored in a static table.  Subsequent calls use that
   table, so the function works for any order of calls and any n ≥ 0. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* The integer part of √887 (a(0)).  887 is a fixed constant. */
    static seq_t a0;

    /* Periodic part of the continued fraction (length 12 for √887). */
    static seq_t period[12];
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Compute a0 = floor(√887). */
        const seq_t D = (seq_t)887ULL;
        a0 = isqrt_seq(D);

        /* Compute the periodic part using the standard algorithm. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        size_t idx = 0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[idx++] = a;
        } while (a != 2 * a0);   /* 2*a0 signals the end of the period */

        period_len = idx;   /* should be 12 for √887 */
        initialized = 1;
    }

    if (n == 0)
        return a0;
    else
        return period[(size_t)((n - 1ULL) % period_len)];
}
