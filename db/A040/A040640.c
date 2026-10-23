/*
 * A040640  Continued fraction for sqrt(666).
 * https://oeis.org/A040640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(666).
   a(0) = floor(sqrt(666)) = 25.
   The periodic part is generated algorithmically and stored
   in a static array. */
#include <stddef.h>

static seq_t a0_val = 0;              /* a(0) */
static seq_t period[200];            /* periodic part, up to 200 terms */
static idx_t period_len = 0;          /* length of the period */
static int initialized = 0;          /* flag: period computed? */

static void init_period(void)
{
    if (initialized) return;
    initialized = 1;

    /* compute a0 = floor(sqrt(666)) */
    seq_t t = 0;
    while ((t + 1) * (t + 1) <= 666) t++;
    a0_val = t;

    /* generate the periodic part */
    seq_t m = 0, d = 1, a = a0_val;
    idx_t len = 0;
    while (1) {
        seq_t m_next = d * a - m;
        seq_t d_next = (666 - m_next * m_next) / d;
        seq_t a_next = (a0_val + m_next) / d_next;

        period[len++] = a_next;

        /* the period ends when a_next == 2 * a0_val */
        if (a_next == 2 * a0_val) break;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    period_len = len;
}

extern seq_t a(idx_t n)
{
    init_period();

    if (n == 0) return a0_val;

    idx_t pos = (n - 1) % period_len;  /* index into the periodic part */
    return period[pos];
}
