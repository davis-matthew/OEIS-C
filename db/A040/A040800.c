/*
 * A040800  Continued fraction for sqrt(829).
 * https://oeis.org/A040800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28, and for n>0 a(n) is the nth term of the simple
 *            continued * fraction for √829. The terms repeat with the period
 *            of the continued * fraction expansion
 * Range:     u8 n <= 0, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t D   = (seq_t)829;   /* the radicand */
static const seq_t a0  = (seq_t)28;    /* floor(sqrt(D)) */

/* Static storage for the periodic part of the continued fraction */
static seq_t period[200];
static size_t period_len = 0;
static int initialized = 0;

/* Compute the periodic part of the continued fraction for √D.
 * The period ends when a_k = 2*a0 (the standard property of simple CFs). */
static void compute_period(void)
{
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    size_t idx = 0;
    while (1) {
        seq_t m1 = d * a - m;
        seq_t d1 = (D - m1 * m1) / d;
        seq_t a1 = (a0 + m1) / d1;

        period[idx++] = a1;
        if (a1 == (a0 << 1)) break;   /* a_k == 2*a0   */

        m = m1;
        d = d1;
        a = a1;
    }
    period_len = idx;
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0u) return a0;
    if (!initialized) compute_period();

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
