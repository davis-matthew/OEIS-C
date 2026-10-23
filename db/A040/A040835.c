/*
 * A040835  Continued fraction for sqrt(865).
 * https://oeis.org/A040835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(865).  a(0) = ⌊√865⌋ = 29, afterwards the terms repeat with period 19. */
#include <stdlib.h>

static const seq_t a0 = 29;            /* first term of the CF */

/* Build the period of the continued fraction of sqrt(865).  The period
 * ends when the generated term equals 2*a0 (i.e. 58).  The algorithm
 * follows the standard recurrence for quadratic surds.  All
 * intermediate arithmetic that could overflow seq_t is performed with
 * idx_t, which is at least 64‑bit. */
static void build_period(seq_t *period, size_t *len)
{
    const idx_t D       = 865;          /* the square‑free part          */
    const idx_t a0k     = (idx_t)a0;    /* first term, duplicated as idx_t */
    const idx_t terminator = 2 * a0k;    /* 58                               */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0k;

    size_t i = 0;
    do
    {
        /* compute next term of the continued fraction */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0k + m) / d;

        /* store it */
        period[i++] = (seq_t)a;
    } while (a != terminator);

    *len = i;
}

/* Sequence A040835 */
seq_t a(idx_t n)
{
    if (n == 0)
        return a0;

    /* period length is small (19) so we recompute it on each call. */
    seq_t period[64];
    size_t len;
    build_period(period, &len);

    /* (n-1) modulo the period length */
    idx_t idx = (n - 1) % (idx_t)len;
    return period[idx];
}
