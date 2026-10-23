/*
 * A040743  Continued fraction for sqrt(771).
 * https://oeis.org/A040743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the (n+1)-th term of the continued fraction expansion of
 *            sqrt(771)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t A0 = (seq_t)27;         /* floor(sqrt(771)) */

/* buffer for the periodic part after a0 */
static seq_t period[128];
static size_t period_len = 0;
static int period_computed = 0;

/* compute the periodic part of the continued fraction for sqrt(771) */
static void compute_period(void)
{
    const int D   = 771;   /* the radicand */
    const int a0  = 27;    /* floor(sqrt(D)) */

    long long M      = 0;   /* m_k          */
    long long D_k    = 1;   /* d_k          */
    long long a_prev = a0;  /* a_k          */

    while (1)
    {
        long long M_next = D_k * a_prev - M;
        long long D_next = (D - M_next * M_next) / D_k;
        long long a_next = (a0 + M_next) / D_next;   /* always use a0 */

        period[period_len++] = (seq_t)a_next;

        if (a_next == 2 * a0)      /* 2*a0 marks the end of the period */
            break;

        M      = M_next;
        D_k    = D_next;
        a_prev = a_next;
    }
}

/* sequence function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) */
        return A0;

    if (!period_computed)      /* initialise once */
    {
        compute_period();
        period_computed = 1;
    }

    /* terms after a0 repeat with the computed period */
    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
