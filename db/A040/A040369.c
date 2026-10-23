/*
 * A040369  Continued fraction for sqrt(389).
 * https://oeis.org/A040369
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

/* Continued fraction for sqrt(389) */

#include <stdbool.h>
#include <stddef.h>

static seq_t a0_val          = 0;
static seq_t period[16]      = {0};
static size_t period_len     = 0;
static bool  period_inited   = false;

static void init_period(void)
{
    if (period_inited)
        return;

    const idx_t D  = 389;
    const idx_t a0 = 19;                     /* floor(sqrt(389)) */
    a0_val = (seq_t)a0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Generate the repeating part; terminate when a == 2*a0,
       which is the last term in the period. */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = (seq_t)a;
    } while (a != a0 * 2);

    period_inited = true;
}

seq_t a(idx_t n)
{
    if (!period_inited)
        init_period();

    if (n == 0)
        return a0_val;

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
