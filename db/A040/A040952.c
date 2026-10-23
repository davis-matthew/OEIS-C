/*
 * A040952  Continued fraction for sqrt(984).
 * https://oeis.org/A040952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms of sqrt(984)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t period[10];
static bool period_initialized = false;

static void init_period(void)
{
    if (period_initialized)
        return;
    period_initialized = true;

    const seq_t a0 = 31;          /* floor(sqrt(984)) */
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a  = a0;
    size_t i = 0;

    for (;;)
    {
        /* compute next triple (m,d,a) */
        unsigned long long u_m = (unsigned long long)d * (unsigned long long)a
                                - (unsigned long long)m;
        m = (seq_t)u_m;

        unsigned long long num_d = 984ULL
                                   - (unsigned long long)m * (unsigned long long)m;
        d = (seq_t)(num_d / (unsigned long long)d);

        unsigned long long num_a = (unsigned long long)a0 + (unsigned long long)m;
        a = (seq_t)(num_a / (unsigned long long)d);

        period[i++] = a;

        if (d == 1 && m == a0)
            break;          /* period ends */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 31;            /* a(0) */

    init_period();

    size_t idx = (size_t)((n - 1) % 10);
    return period[idx];
}
