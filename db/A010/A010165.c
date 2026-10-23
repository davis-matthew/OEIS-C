/*
 * A010165  Continued fraction for sqrt(94).
 * https://oeis.org/A010165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued‑fraction terms of sqrt(94) a(0) = 9, a(1..16) = 1, 2,
 *            3, 1, 1, 5, 1, 8, 1, 5, 1, 1, 3, 2, 1, 18, then the period
 *            repeats
 * Range:     u8 n <= 9813, u16 n <= 9813, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t period[64];
static size_t period_len = 0;

static void compute_period(void)
{
    const idx_t S = 94;          /* radicand */
    const idx_t a0 = 9;          /* floor(sqrt(94)) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;                /* current a_k */
    size_t i = 0;

    while (1) {
        /* Next m: m_{k+1} = d_k * a_k - m_k */
        m = d * a - m;
        /* Next d: d_{k+1} = (S - m_{k+1}^2) / d_k */
        d = (S - m * m) / d;
        /* Next a: a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        idx_t a_next = (a0 + m) / d;
        period[i++] = (seq_t)a_next;
        if (a_next == 2 * a0)    /* last term of the period */
            break;
        a = a_next;              /* advance to the next step */
    }
    period_len = i;
}

seq_t a(idx_t n)
{
    if (period_len == 0)         /* build period on first call */
        compute_period();

    if (n == 0)
        return (seq_t)9;         /* a(0) = 9 */

    /* The periodic part starts at a(1). */
    size_t idx = (size_t)((n - 1) % (idx_t)period_len);
    return period[idx];
}
