/*
 * A040613  Continued fraction for sqrt(639).
 * https://oeis.org/A040613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   int_floorsqrt_639
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Continued fraction for sqrt(639)
 * a(0) = ⌊√639⌋ = 25
 * a(n) = continued fraction coefficients for √639   (n ≥ 1)
 * The sequence is periodic after the first term.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_floorsqrt_639.h"

/* static cache for the period */
static bool initialized = false;
static seq_t a0;               /* first term (floor of sqrt(639)) */
static seq_t period[200];      /* period storage, safe for all n */
static size_t period_len = 0;  /* length of period */

/* compute the period of the continued fraction of sqrt(639) */
static void init_period(void)
{
    a0 = int_floorsqrt_639();   /* 25 */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    period_len = 0;

    for (;;) {
        /* recurrence for continued fractions of sqrt(N) */
        m = d * a - m;
        d = ((seq_t)639 - m * m) / d;
        a = (a0 + m) / d;

        period[period_len++] = a;

        if (a == 2 * a0)       /* period ends when a == 2*a0 */
            break;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_period();

    if (n == 0)
        return a0;                     /* first term */

    /* subsequent terms repeat the period */
    return period[(n - 1) % period_len];
}
