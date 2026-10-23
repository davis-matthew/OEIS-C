/*
 * A040362  Continued fraction for sqrt(382).
 * https://oeis.org/A040362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) gives the nth coefficient of the simple continued fraction
 *            expansion of sqrt(382); a(0) = floor(sqrt(382)) = 19
 * Range:     u8 n <= 0, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic after the first term. The period length L is
   determined by generating the continued‑fraction terms until the
   coefficient equals 2*a0 (which is 38 for sqrt(382)).  All indices
   are 0‑based.  The implementation memoises the period and returns
   a(n) in O(1) time for any n >= 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t _period_len = 0;           /* number of periodic coefficients (a1..aL) */
static seq_t *_period = NULL;            /* array holding the periodic coefficients */
static bool _period_computed = false;    /* flag */

static void _compute_period(void)
{
    /* Only compute once */
    if (_period_computed) return;
    _period_computed = true;

    /* Coefficients are all small; an upper bound on period length is 2*sqrt(382)+1 < 80.
       We start with a modest capacity and resize if necessary. */
    size_t cap = 64;
    _period = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!_period) return; /* Allocation failure -> sequence will be incomplete */

    const seq_t D = 382;
    seq_t a0 = 19;        /* floor(sqrt(382)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    seq_t two_a0 = a0 * (seq_t)2;

    size_t pos = 0;
    /* Generate terms a1, a2, ..., until we reach a == 2*a0 */
    do {
        /* Compute next m, d, a */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;        /* exact division */
        seq_t a_next = (a0 + m_next) / d_next;           /* floor division */

        /* Store a_next in period array */
        if (pos >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(_period, cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure: truncate period to current length */
                _period_len = pos;
                return;
            }
            _period = tmp;
        }
        _period[pos++] = a_next;

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;
    } while (a != two_a0);

    /* Reserve only the needed length */
    seq_t *tmp = (seq_t *)realloc(_period, pos * sizeof(seq_t));
    if (tmp) _period = tmp;
    _period_len = pos;
}

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(382). */
    if (n == 0u) return (seq_t)19;

    /* Ensure the periodic part is computed. */
    if (!_period_computed) _compute_period();

    /* Period repeats from a(1) onwards. */
    size_t idx = (size_t)(n - 1u) % _period_len;
    return _period[idx];
}
