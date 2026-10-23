/*
 * A040259  Continued fraction for sqrt(276).
 * https://oeis.org/A040259
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

/* Continued fraction terms for sqrt(276)
   a(0) = floor(sqrt(276)) and for n>=1, a(n) repeats the period
   1,1,1,1,2,2,2,1,1,1,1,32. */
#include <stdbool.h>
#include <stddef.h>

/* forward declaration required by the build system */
seq_t a(idx_t n);

/* static storage for the period */
static bool initialized = false;
static seq_t a0;                   /* the first term a(0) = floor(sqrt(276)) */
static seq_t period[64];           /* large enough for the period of sqrt(276) */
static size_t period_len = 0;

/* initialise the period once */
static void init_period(void)
{
    if (initialized) return;

    /* first term a0 = floor(sqrt(276)) */
    a0 = 0;
    for (seq_t i = 0; i * i <= 276; ++i) {
        a0 = i;
    }

    /* continued‑fraction algorithm for sqrt(276) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;

    while (1) {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (276 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        period[period_len++] = a_next;
        if (a_next == 2 * a0) break;   /* end of the period */

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_period();

    if (n == 0) {
        return a0;
    }

    /* for n >= 1, the period starts at a(1) */
    return period[(n - 1) % period_len];
}
