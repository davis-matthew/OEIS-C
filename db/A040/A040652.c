/*
 * A040652  Continued fraction for sqrt(679).
 * https://oeis.org/A040652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040652 – Continued fraction for sqrt(679)
 *
 *  The sequence gives the partial quotients of the simple continued
 *  fraction expansion of sqrt(679).  The first term a(0) is
 *  floor(sqrt(679)) = 26.  The remaining terms are periodic with
 *  period 16: 17, 2, 1, 5, 8, 1, 1, 25, 1, 1, 8, 5, 1, 2, 17, 52.
 *  The algorithm below computes the period once and returns any
 *  term in O(1) time, suitable for static linkage in the test
 *  harness.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  a(0) is the integer part of sqrt(679)  */
    static const seq_t a0 = 26;             /* floor(sqrt(679)) */
    static seq_t *period = NULL;           /* buffer for the periodic block */
    static size_t period_len = 0;           /* length of the period */
    static const seq_t two_a0 = a0 + a0;    /* 52, the terminating term */

    /*  Build the period only once.  */
    if (period_len == 0) {
        const seq_t D = 679;                /* the radicand */
        seq_t m      = 0;                   /* m_0 */
        seq_t d      = 1;                   /* d_0 */
        seq_t a_cur  = a0;                  /* a_0 */

        /*  Allocate a small buffer – the period is at most 32.  */
        period = (seq_t *)malloc(32 * sizeof(seq_t));
        size_t len = 0;

        for (;;) {
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[len++] = a_next;
            if (a_next == two_a0)            /* 2*a0 marks the end of the period */
                break;

            m = m_next;
            d = d_next;
            a_cur = a_next;
        }
        period_len = len;                    /* store the period length */
    }

    /*  Handle the zeroth term directly.  */
    if (n == 0)
        return a0;

    /*  All subsequent terms are periodic.  */
    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
