/*
 * A040198  Continued fraction for sqrt(213).
 * https://oeis.org/A040198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of √213 */
/* a(0) = ⌊√213⌋ = 14, thereafter the period is */
/* 1, 1, 2, 6, 1, 8, 1, 6, 2, 1, 1, 28 (period length 12) */

#include <stddef.h>          /* for size_t */

static const seq_t RAD = (seq_t)213;       /* radicand */
static seq_t period[64];                  /* periodic block (a1 … a_k)  */
static size_t period_len = 0;             /* number of period elements */
static unsigned period_inited = 0;        /* flag whether period was built */

/* Build the continued‑fraction period for √213 once. */
static void init_period(void)
{
    if (period_inited) return;            /* already computed */

    const seq_t a0 = (seq_t)14;           /* floor(√213)                 */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    do {
        /* standard recurrence for continued fractions of square roots */
        const seq_t m_next = d * a - m;
        const seq_t d_next = (RAD - m_next * m_next) / d;
        const seq_t a_next = (a0 + m_next) / d_next;

        period[period_len++] = a_next;    /* store the term */

        if (a_next == (seq_t)(2 * a0))    /* 2·a0 signals end of period      */
            break;                       /* include this final term in period*/

        /* advance to next triplet (m,d,a) */
        m = m_next;
        d = d_next;
        a = a_next;
    } while (1);

    period_inited = 1;
}

/* OEIS sequence A040198: continued-fraction terms of √213 */
seq_t a(idx_t n)
{
    init_period();                        /* ensure period is ready */

    if (n == 0) return (seq_t)14;         /* a(0) is the integer part */

    const idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];           /* periodic part for n ≥ 1 */
}
