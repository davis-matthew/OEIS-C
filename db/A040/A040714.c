/*
 * A040714  Continued fraction for sqrt(742).
 * https://oeis.org/A040714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth term of the continued fraction expansion of sqrt(742)
   (OEIS sequence A040714).  */
# include <stddef.h>

static seq_t period[128];
static size_t period_len = 0;
static void init_period(void)
{
    if (period_len != 0)
        return;
    const seq_t D  = 742;          /* the radicand */
    const seq_t a0 = 27;           /* floor(sqrt(742)) */
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a  = a0;
    size_t i = 0;

    while (1)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[i++] = a;
        if (a == 2 * a0)     /* the period ends when a == 2*a0 */
            break;
        /* the period for sqrt(742) is only 14, so 128 is ample */
    }
    period_len = i;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 27;           /* the integer part of sqrt(742) */
    init_period();
    /* the sequence after the first term repeats with period_len elements */
    size_t k = (size_t)((n - 1) % period_len);
    return period[k];
}
