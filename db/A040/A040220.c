/*
 * A040220  Continued fraction for sqrt(236).
 * https://oeis.org/A040220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 15; for n > 0 the terms are the coefficients of the
 *            periodic continued fraction expansion of sqrt(236) after the
 *            first term
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static idx_t period_len = 0;
static seq_t period[128];

static void init_period(void)
{
    if (period_len > 0) return;              /* already initialized */

    const seq_t D  = 236;                     /* the square‑free part */
    const seq_t a0 = 15;                      /* floor(sqrt(236)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;                             /* current coefficient */

    do {
        /* m_{k+1} = d_k * a_k - m_k   (a_k is the previous coefficient) */
        m = d * a - m;
        /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        d = (D - m * m) / d;
        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        a = (a0 + m) / d;
        period[period_len++] = a;             /* store a1 … a_{period} (a_{period}=2*a0) */
    } while (a != 2 * a0);                   /* stop after first 2*a0 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 15;                    /* a(0) = floor(sqrt(236)) */
    init_period();                            /* ensure period array is ready */
    idx_t k = (n - 1) % period_len;           /* index into period */
    return period[k];
}
