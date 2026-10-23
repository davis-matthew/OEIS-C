/*
 * A041452  Numerators of continued fraction convergents to sqrt(242).
 * https://oeis.org/A041452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numerators of continued fraction convergents to √242.
 * The sequence starts with a(0) = 15.
 *
 * The algorithm:
 *   1. Compute the period of the continued fraction of √242.
 *   2. Use the recurrence
 *        p_{-2} = 0,  p_{-1} = 1
 *        p_k   = a_k · p_{k-1} + p_{k-2}   (k ≥ 0)
 *        where a_0 = 15 and a_k (k ≥ 1) are the periodic terms.
 *   3. Return p_n, the numerator of the n‑th convergent.
 *
 * The implementation works for any integer type seq_t (uint8_t, uint16_t,
 * uint32_t, uint64_t, __int128, or a big‑integer class) and all idx_t
 * values, with no assumptions about the order of calls.
 */

#include <stdbool.h>
#include <stddef.h>

#define PERIOD_CAP 200

static idx_t period[PERIOD_CAP];
static size_t period_len = 0;
static bool period_initialised = false;

/* Initialise the period of the continued‑fraction expansion of √242. */
static void init_period(void)
{
    if (period_initialised)
        return;

    const idx_t N      = 242;
    const idx_t a0     = 15;           /* ⌊√242⌋ */
    idx_t m = 0, d = 1, a = a0;
    size_t i = 0;

    while (1) {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;

        period[i++] = a;
        if (a == 2 * a0)          /* end of period */
            break;

        /* safety guard – period should never exceed PERIOD_CAP */
        if (i >= PERIOD_CAP)
            break;
    }

    period_len            = i;
    period_initialised    = true;
}

/* Return the nth term of the sequence (a(0) = 15). */
seq_t a(idx_t n)
{
    init_period();

    seq_t p_prevprev = 0;   /* p_{-2} */
    seq_t p_prev     = 1;   /* p_{-1} */
    seq_t p_curr;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;

        if (k == 0) {
            ak = (seq_t)15u;                           /* a_0 */
        } else {
            ak = (seq_t)period[(k - 1) % period_len]; /* a_k, k ≥ 1 */
        }

        p_curr = ak * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_prev;   /* p_n is the numerator of the nth convergent */
}
