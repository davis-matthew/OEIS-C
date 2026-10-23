/*
 * A040775  Continued fraction for sqrt(804).
 * https://oeis.org/A040775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040775: Continued fraction for sqrt(804)
 *  a(0) = 28
 *  for n >= 1 the terms repeat the pattern
 *  2,1,4,2,18,2,4,1,2,56  (period length 10)
 */
#include <stdint.h>
#include <stddef.h>

static seq_t period[10];
static size_t period_len = 0;
static int period_init = 0;

/* compute the periodic part of sqrt(804) and store in period[] */
static void init_period(void)
{
    const uint64_t D = 804ULL;
    /* integer sqrt of D */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        a0++;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* generate periodic terms until a == 2*a0 */
    while (1)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[period_len++] = (seq_t)a;

        if (a == 2 * a0)
            break;
    }
    period_init = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    if (!period_init)
        init_period();

    idx_t idx = n - 1;                /* skip the integer part */
    size_t pos = (size_t)(idx % period_len);
    return period[pos];
}
