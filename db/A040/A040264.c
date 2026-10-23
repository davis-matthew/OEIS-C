/*
 * A040264  Continued fraction for sqrt(281).
 * https://oeis.org/A040264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=16 (floor(sqrt(281))); for n≥1, a(n) is the (n)-th
 *            coefficient of the continued‑fraction expansion of sqrt(281)
 *            after the integer part
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const idx_t D = 281;                /* Radicand */
static const seq_t a0_value = (seq_t)16;   /* floor(sqrt(281)) */

static seq_t period[200];                  /* Periodic part */
static idx_t period_len = 0;
static bool initialized = false;

/* Compute the continued‑fraction period for sqrt(D). */
static void compute_period(void)
{
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = (idx_t)16;          /* a0 */
    idx_t a0 = a;

    /* Generate terms until the last term 2*a0 is produced. */
    do
    {
        m = d * a - m;
        d = (idx_t)((D - m * m) / d);
        a = (idx_t)((a0 + m) / d);
        period[period_len++] = (seq_t)a;
    } while (a != 2 * a0);
}

seq_t a(idx_t n)
{
    /* Lazily compute the period on first use. */
    if (!initialized)
    {
        compute_period();
        initialized = true;
    }

    if (n == 0)
        return a0_value;                     /* a(0) = 16 */

    /* Subsequent terms repeat with period length. */
    idx_t p = (n - 1) % period_len;           /* zero‑based within period */
    return period[p];
}
