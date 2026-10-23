/*
 * A040409  Continued fraction for sqrt(430).
 * https://oeis.org/A040409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040409: coefficients of the continued fraction for sqrt(430) */
#include <stdint.h>

/* Requested function */
seq_t a(idx_t n)
{
    /* Periodic part starts with the first coefficient after the initial term */
    static seq_t period[200];
    static idx_t period_len = 0;

    if (period_len == 0) /* initialise once */
    {
        const seq_t a0 = 20;          /* floor(sqrt(430)) */
        const idx_t D   = 430;        /* the non‑perfect square */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        /* generate periodic coefficients until the last one equals 2*a0 */
        while (1)
        {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a - m;
            /* d_{k+1} = (D - m_{k+1}²) / d_k   (non‑negative as guaranteed) */
            seq_t num = (seq_t)D - m * m;
            d = num / d;
            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
            a = (a0 + m) / d;

            period[period_len++] = a;
            if (a == 2 * a0)       /* period finished (a_k = 2*a0) */
                break;
            /* safety guard – period for N=430 is 14 */
            if (period_len >= 200)
                break;
        }
    }

    if (n == 0)   /* initial term */
        return 20; /* floor(sqrt(430)) */

    /* periodic part (n-1)%period_len gives index into period array */
    const idx_t idx = (n - 1) % period_len;
    return period[idx];
}
