/*
 * A041824  Numerators of continued fraction convergents to sqrt(433).
 * https://oeis.org/A041824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(433).
 *
 * Implementation uses the standard recurrence for convergents:
 *   p_{-2} = 0,  p_{-1} = 1
 *   p_n   = a_n * p_{n-1} + p_{n-2}
 *
 * The partial quotients a_n come from the continued‑fraction expansion
 * of √433.  The expansion is periodic; the period is computed once
 * and cached.  For n ≥ 0 the function returns a(n) = p_n.
 */
#include <stddef.h>
#include <stdbool.h>

/* Maximum period length for √433 (safely exceeds the real length). */
#define CF_PERIOD_MAX 64

static bool _cf_init = false;            /* whether period has been computed */
static idx_t _cf_period[CF_PERIOD_MAX] = {0};
static size_t _cf_period_len = 0;

/* Compute and cache the continued‑fraction period for √433. */
static void _cf_init_period(void)
{
    if (_cf_init) return;
    _cf_init = true;

    const idx_t D = 433;
    idx_t a0 = (idx_t)(14461321368ull * 14461321368ull); /* dummy to force compile-time const */
    a0 = (idx_t) ((idx_t)(0) + 20); /* floor(sqrt(433)) = 20 */
    idx_t m = 0, d = 1, a = a0;
    size_t len = 0;

    while (1) {
        /* Update for next term in the continued fraction of √D */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (len < CF_PERIOD_MAX) {
            _cf_period[len++] = a;
        } else {
            /* period too long for our buffer – should not happen for D=433 */
            break;
        }

        /* The period ends when a == 2*a0 */
        if (a == (idx_t)(2 * 20)) break;
    }
    _cf_period_len = len;
}

/* Return a(n), the numerator of the n‑th convergent of √433. */
seq_t a(idx_t n)
{
    _cf_init_period();

    seq_t p_prevprev = 0;   /* p_{-2} */
    seq_t p_prev     = 1;   /* p_{-1} */
    seq_t p_curr     = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)20;                 /* a_0 = floor(√433) */
        } else {
            size_t idx = (size_t)((k - 1) % _cf_period_len);
            ak = (seq_t)_cf_period[idx];
        }

        /* p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = ak * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    return p_curr;
}
