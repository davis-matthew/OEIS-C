/*
 * A040260  Continued fraction for sqrt(277).
 * https://oeis.org/A040260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n‑th coefficient of the continued fraction expansion
 *            of √277: a(0)=⌊√277⌋=16; the remaining terms repeat with period
 *            until an element equals 2*16=32
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required function */
#include <stddef.h>  /* for size_t */

static seq_t period_arr[256];
static size_t period_len = 0;

/* Construct the periodic part of the continued fraction for √277. */
static void init_period(void)
{
    if (period_len != 0)
        return;

    const idx_t D = 277;
    idx_t m  = 0;          /* m_k   */
    idx_t d  = 1;          /* d_k   */
    idx_t a0 = 16;         /* floor(√277) */
    idx_t a  = a0;         /* current a_k */
    size_t i = 0;

    while (1) {
        /* produce next a_k values */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period_arr[i++] = (seq_t)a;

        if (a == 2 * a0)   /* terminate when a_k == 2*a0 */
            break;
    }
    period_len = i;
}

/* Return the nth term of the sequence (zero‑based). */
seq_t a(idx_t n)
{
    init_period();

    if (n == 0)
        return (seq_t)16;            /* a(0) */

    size_t idx = (size_t)((n - 1) % period_len);
    return period_arr[idx];
}
