/*
 * A040647  Continued fraction for sqrt(673).
 * https://oeis.org/A040647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for √673.
   a(0) = ⌊√673⌋ = 25.
   For n ≥ 1, a(n) is the nth coefficient after the initial term
   of the simple continued‑fraction expansion of √673.  The period
   terminates when a coefficient equals 2·a(0).  The algorithm
   follows the standard recurrence (see e.g. Abramowitz & Stegun). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t a0_val;            /* a(0) = ⌊√673⌋ */
static seq_t period[256];       /* storage for periodic coefficients */
static idx_t period_len = 0;    /* number of elements in period */

/* Initialise a0 and find the period once. */
static void
init_period(void)
{
    if (period_len != 0)          /* already initialised */
        return;

    /* a0 as seq_t and as idx_t */
    a0_val = (seq_t)_floor_isqrt_u64(673);
    idx_t a0_small = (idx_t)a0_val;      /* 25 */
    idx_t twice_a0  = 2 * a0_small;      /* 50 */

    /* Continued‑fraction variables */
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a   = a0_small;

    while (true) {
        /* Recurrence to generate the next triple (m,d,a). */
        idx_t m_next = d * a - m;
        idx_t d_next = ((idx_t)673 - m_next * m_next) / d;
        /* Compute a_next as a seq_t and store it. */
        seq_t a_next = (a0_val + (seq_t)m_next) / (seq_t)d_next;

        period[period_len++] = a_next;

        /* Terminate when the coefficient equals 2·a(0). */
        if ((idx_t)a_next == twice_a0)
            break;

        /* Update for next iteration. */
        m = m_next;
        d = d_next;
        a = (idx_t)a_next;   /* a_next fits into idx_t, being ≤ 2·a0 */
    }
}

/* Return the nth term of the sequence. */
seq_t
a(idx_t n)
{
    init_period();                 /* ensure a0 and period are ready */

    if (n == 0)
        return a0_val;             /* the initial term */

    /* Periodic part: n-1 is 0‑based index in period[]. */
    return period[(n - 1) % period_len];
}
