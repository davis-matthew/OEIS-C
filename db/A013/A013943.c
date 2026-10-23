/*
 * A013943  Period of continued fraction for sqrt(m), m = n-th nonsquare.
 * https://oeis.org/A013943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = period of the continued fraction of sqrt(m), where m is
 *            the nth non‑square integer
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* --- find the nth nonsquare m --- */
    idx_t m = n;                          /* start at n (definitely ≤ m)   */
    while (1) {
        seq_t msq = (seq_t)m;             /* convert to seq_t safely       */
        seq_t s   = floor_sqrt(msq);      /* floor(sqrt(m))                */
        seq_t non = msq - s;              /* count of nonsquares ≤ m       */
        if (non == (seq_t)n)              /* reached the nth nonsquare?    */
            break;
        m++;                              /* try next integer              */
    }

    seq_t mseq = (seq_t)m;                /* value of m as seq_t            */
    seq_t a0   = floor_sqrt(mseq);       /* integer part of sqrt(m)        */

    /* --- compute the period of the continued fraction of sqrt(m) --- */
    seq_t set_a = a0;                     /* previous a_i value             */
    seq_t set_m = 0;                      /* previous m_i value             */
    seq_t set_d = 1;                      /* previous d_i value             */
    seq_t period = 0;

    while (1) {
        seq_t m_next = set_d * set_a - set_m;
        seq_t d_next = (mseq - m_next * m_next) / set_d;
        seq_t a_next = (a0 + m_next) / d_next;
        period++;

        if (a_next == a0 * 2)            /* period ends when a_i = 2*a0    */
            break;

        set_m = m_next;                  /* advance to next step           */
        set_d = d_next;
        set_a = a_next;
    }

    return period;
}
