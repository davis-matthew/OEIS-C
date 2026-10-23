/*
 * A042654  Numerators of continued fraction convergents to sqrt(857).
 * https://oeis.org/A042654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   compute the numerators of the convergents to sqrt(857)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A standard algorithm for the simple continued fraction of √D is used.
   The period of the continued fraction for √857 is computed once on first
   use. The recurrence a_k = c_k * a_{k-1} + a_{k-2} is then applied
   using those partial quotients.  All arithmetic is done with the
   provided seq_t type, with no floating point operations. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *period = NULL;      /* store partial quotients after a0 */
static idx_t period_len = 0;       /* number of terms in the period */
static bool period_ready = false;

static void init_period(void)
{
    if (period_ready)
        return;

    const idx_t D = 857;            /* discriminant */
    /* compute a0 = ⌊√D⌋ */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        ++a0;

    /* allocate an initial buffer for the period terms */
    size_t cap = 16;
    period = (seq_t*)malloc(cap * sizeof(seq_t));

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_cur = a0;

    /* generate the period until the term 2*a0 appears */
    while (true)
    {
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;

        if (period_len == cap) {
            cap <<= 1;
            period = (seq_t*)realloc(period, cap * sizeof(seq_t));
        }
        period[period_len++] = (seq_t)a_cur;

        if (a_cur == 2 * a0)
            break;
    }

    period_ready = true;
}

seq_t a(idx_t n)
{
    init_period();

    /* compute a0 again for the recurrence; it matches the value used
       during period generation because the calculation is deterministic. */
    const idx_t D = 857;
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        ++a0;

    seq_t p_prev2 = 0;   /* corresponds to p_{-2} */
    seq_t p_prev1 = 1;   /* corresponds to p_{-1} */
    seq_t current;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)a0;                /* first partial quotient */
        } else {
            idx_t idx = (k - 1) % period_len; /* period repeats afterwards */
            ak = period[idx];
        }
        current = ak * p_prev1 + p_prev2; /* recurrence */
        p_prev2 = p_prev1;
        p_prev1 = current;
    }
    return p_prev1;   /* a(n) = p_n */
}
