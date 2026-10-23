/*
 * A010160  Continued fraction for sqrt(88).
 * https://oeis.org/A010160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) gives the n-th term of the continued fraction of sqrt(88).
 *            The algorithm computes the simple continued fraction of sqrt(88)
 *            and repeats its period. The first term a(0) is the integer part
 *            of sqrt(88), and for n > 0, a(n) = period[(n-1) % period_len]
 *            where "period" = {2,1,1,1,2,18}
 * Range:     u8 n <= 9039, u16 n <= 9039, u32 n <= 9039, u64 n <= 9039,
 *            u128 n <= 9039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t period[10];
static size_t period_len = 0;
static int period_initialized = 0;

static void init_period(void)
{
    if (period_initialized) return;
    period_initialized = 1;

    const seq_t D = (seq_t)88;       /* discriminant */
    const seq_t a0 = (seq_t)9;       /* floor(sqrt(88)) */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    size_t len = 0;

    for (;;)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[len++] = a;
        if (a == a0 * 2)            /* end of period (a == 2*a0) */
            break;
    }
    period_len = len;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)9;            /* integer part of sqrt(88) */

    init_period();

    /* Index into the periodic part */
    idx_t idx = n - (idx_t)1;
    size_t pos = (size_t)(idx % period_len);
    return period[pos];
}
