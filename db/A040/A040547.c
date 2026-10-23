/*
 * A040547  Continued fraction for sqrt(571).
 * https://oeis.org/A040547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n-th term of the continued fraction expansion of
 *            sqrt(571) For n = 0 the value is floor(sqrt(571)), then the
 *            sequence of partial quotients repeats after a strictly periodic
 *            cycle. The sequence is generated with the standard algorithm for
 *            square‑root continued fractions
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   _grow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_grow_seq_t.h"

static bool _cf_sqrt571_initialized = false;
static seq_t _a0;            /* floor(sqrt(571))                          */
static size_t _period_len;   /* number of terms in the period (>=1)       */
static seq_t *_period = NULL;/* array holding the periodic terms a(1..)   */

/* Compute the periodic part of the continued fraction for sqrt(571) */
static void _compute_cf_sqrt571(void)
{
    const idx_t D = 571;                 /* non‑square integer                     */
    seq_t a0 = 0;                        /* floor(sqrt(D))                          */
    while ((a0 + 1U) * (a0 + 1U) <= (seq_t)D) ++a0;
    _a0 = a0;

    seq_t m = 0, d = 1, a = a0;
    seq_t *vec = NULL;                   /* temporary storage of period terms      */
    size_t cap = 0, used = 0;

    while (1) {
        /* compute next triple (m', d', a') */
        seq_t m_next = d * a - m;
        seq_t d_next = ((seq_t)D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        _grow_seq_t(&vec, &cap, &used);
        vec[used++] = a_next;

        if (a_next == 2 * a0) break;     /* period ends after this term          */

        /* advance */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    _period_len = used;
    _period = vec;
    _cf_sqrt571_initialized = true;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (!_cf_sqrt571_initialized)
        _compute_cf_sqrt571();

    if (n == 0)
        return _a0;                    /* first term a(0)                        */

    /* n>=1: use periodic part */
    size_t idx = ((size_t)(n - 1)) % _period_len;
    return _period[idx];
}
