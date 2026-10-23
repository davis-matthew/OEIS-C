/*
 * A040283  Continued fraction for sqrt(301).
 * https://oeis.org/A040283
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

/* Continued fraction terms of sqrt(301) (cf[0] = floor(sqrt(301))) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t period[32];
static size_t period_len = 0;
static bool initialized = false;

static void init_period(void)
{
    const idx_t D = 301;
    const idx_t a0 = 17;          /* floor(sqrt(301)) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    size_t i = 0;

    while (1)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[i++] = (seq_t)a;
        if (a == 2 * a0)   /* period ends when a == 2*a0 */
            break;
    }
    period_len = i;
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        init_period();
        initialized = true;
    }

    if (n == 0)
        return (seq_t)17;   /* a(0) = floor(sqrt(301)) */

    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}
