/*
 * A040830  Continued fraction for sqrt(860).
 * https://oeis.org/A040830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   terms of the continued fraction expansion of sqrt(860), i.e.
 *            a(n) is the nth partial quotient of sqrt(860) (a(0) = ⌊√860⌋)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_sqrt_860
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt_860.h"

/* static storage for the computed period */
static bool init_done = false;
static seq_t period[128];   /* will contain a(0), a(1), ... , a(0)+period length×2  */
static idx_t period_len = 0; /* number of terms stored, including a(0) */

/* initialise period terms for sqrt(860) the first time a() is called */
static void init_period(void)
{
    const idx_t D = 860;
    seq_t a0 = floor_sqrt_860();   /* 29 */

    /* first term a(0) = integer part */
    period[period_len++] = a0;

    /* generate periodic part */
    seq_t m = 0, d = 1, a = a0;
    while (1) {
        /* standard continued‑fraction recurrence for sqrt(D)  */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[period_len++] = a;   /* store next quotient */

        /* period ends when a equals 2*a0  */
        if (a == 2 * a0) {
            break;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!init_done) {
        init_period();
        init_done = true;
    }

    if (n < period_len) {
        return period[n];
    } else {
        /* after a(0) the pattern repeats with length period_len-1 */
        idx_t i = 1 + ((n - 1) % (period_len - 1));
        return period[i];
    }
}
