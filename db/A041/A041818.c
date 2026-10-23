/*
 * A041818  Numerators of continued fraction convergents to sqrt(430).
 * https://oeis.org/A041818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(430). */
/* Uses recurrence p_-2 = 0, p_-1 = 1, */
/* p_n   = a_n * p_{n-1} + p_{n-2} */
/* CF coefficients: a_0 = 20, and for n>=1 a_n is periodic with period 14 */
/*   [1,2,1,3,1,6,8,6,1,3,1,2,1,40] */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre-compute the periodic part of the continued fraction once */
    static int *period = NULL;
    static size_t period_len = 0;

    const int a0 = 20;          /* floor(sqrt(430)) */

    if (!period)
    {
        /* Generate coefficients until 2*a0 appears (end of period) */
        int m = 0, d = 1, a = a0;
        size_t cap = 8;
        size_t count = 0;
        period = (int *)malloc(cap * sizeof(int));

        for (;;)
        {
            int m_next = d * a - m;
            int d_next = (430 - m_next * m_next) / d;
            int a_next = (a0 + m_next) / d_next;

            if (count >= cap) {
                cap *= 2;
                period = (int *)realloc(period, cap * sizeof(int));
            }
            period[count++] = a_next;

            if (a_next == 2 * a0)
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
        period_len = count;
    }

    /* Recurrence for convergents */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p = 0;

    for (idx_t i = 0; i <= n; ++i) {
        int ai = (i == 0) ? a0 : period[(i - 1) % period_len];
        p = (seq_t)ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
