/*
 * A040388  Continued fraction for sqrt(409).
 * https://oeis.org/A040388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(409)
   a(0) = ⌊√409⌋ = 20.
   For n > 0, a(n) is the (n‑1)th element of the periodic part,
   which ends when the term equals 2·a(0) = 40. */

#include <stdbool.h>
#include <stddef.h>

static seq_t period[128];      /* storage for the periodic part */
static size_t period_len;     /* actual length of the period */

/* Compute the period of the continued fraction expansion of √409. */
static void compute_period(void)
{
    /* initial values */
    const seq_t a0  = (seq_t)20;    /* floor(√409) */
    const seq_t D   = (seq_t)409;   /* the radicand */

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;
    size_t i = 0;

    while (true) {
        /* Compute next values of m, d, and a */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        period[i++] = a_next;          /* store periodic term */
        if (a_next == 2 * a0)          /* termination rule for sqrt(n) */
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }
    period_len = i;
}

/* Main sequence access function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        compute_period();
        initialized = true;
    }

    if (n == 0)
        return (seq_t)20;                       /* first term */

    /* subsequent terms cycle through the period */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
