/*
 * A042541  Denominators of continued fraction convergents to sqrt(799).
 * https://oeis.org/A042541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for denominators of convergents to sqrt(799) */
/* Uses the standard continued‑fraction period of sqrt(799). */

#include <stdlib.h>
#include <stddef.h>

/* Period of the continued fraction for sqrt(799).  Computed once. */
static int *period = NULL;         /* a_1 … a_P */
static size_t period_len = 0;      /* length of the period */

static void init_period(void)
{
    if (period_len > 0)
        return;                    /* already done */

    const idx_t D = 799;           /* target square root */
    const int   a0 = 28;           /* floor(sqrt(799)) */

    idx_t m = 0;
    idx_t d = 1;
    int   a = a0;

    /* dynamic array */
    size_t cap = 8;
    period = (int *)malloc(cap * sizeof(int));
    period_len = 0;

    while (1) {
        /* next values in the standard sqrt algorithm */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        int   a_next = (a0 + m_next) / d_next;

        /* store a_next */
        if (period_len >= cap) {
            cap *= 2;
            period = (int *)realloc(period, cap * sizeof(int));
        }
        period[period_len++] = a_next;

        /* period ends when the coefficient repeats 2*a0 */
        if (a_next == 2 * a0)
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Run‑time cache of denominators q_n.  Each call may extend the cache. */
static seq_t *denoms = NULL;     /* cached q_0 … q_k */
static size_t denoms_cap = 0;    /* current capacity of array */
static size_t denoms_len = 0;    /* numbers already stored */

static seq_t p_prevprev, p_prev;
static seq_t q_prevprev, q_prev;
static size_t next_idx;          /* index to be generated next */

/* Ensure that denoms[0..n] exist */
static void ensure_denoms(idx_t n)
{
    init_period();

    /* Allocate the array on first use */
    if (denoms_cap == 0) {
        denoms_cap = 4;
        denoms = (seq_t *)malloc(denoms_cap * sizeof(seq_t));
    }

    if (denoms_len > n)
        return;                 /* already available */

    /* Initialise recurrence state if this is the first computation */
    if (denoms_len == 0) {
        p_prevprev = (seq_t)0;
        p_prev     = (seq_t)1;
        q_prevprev = (seq_t)1;
        q_prev     = (seq_t)0;
        next_idx   = 0;
    }

    /* Generate terms until we have index n */
    while (denoms_len <= n) {
        int ai = (next_idx == 0) ? 28
                                 : period[(next_idx - 1) % period_len];

        seq_t ai_seq = (seq_t)ai;

        seq_t p = ai_seq * p_prev + p_prevprev;
        seq_t q = ai_seq * q_prev + q_prevprev;

        /* grow array if needed */
        if (denoms_len == denoms_cap) {
            denoms_cap *= 2;
            denoms = (seq_t *)realloc(denoms, denoms_cap * sizeof(seq_t));
        }

        denoms[denoms_len++] = q;

        /* shift recurrence for next iteration */
        p_prevprev = p_prev;
        p_prev     = p;
        q_prevprev = q_prev;
        q_prev     = q;

        ++next_idx;
    }
}

/* Public interface required by the harness. */
seq_t a(idx_t n)
{
    ensure_denoms(n);
    return denoms[n];
}
