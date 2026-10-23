/*
 * A040268  Continued fraction for sqrt(285).
 * https://oeis.org/A040268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: continued‑fraction terms of sqrt(285). */
/* a(0) = floor(sqrt(285)) = 16.  For n > 0 the
   periodic part [1, 7, 2, 7, 1, 32] repeats.  */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial term */
    const seq_t a0 = (seq_t)16;

    /* storage for the periodic part (up to 64 terms is plenty) */
    static size_t period_len = 0;
    static seq_t period[64];

    /* compute period once, lazy initialization */
    if (period_len == 0) {
        const unsigned D = 285;          /* the radicand */
        unsigned m = 0;                  /* m_k */
        unsigned d = 1;                  /* d_k */
        unsigned a = 16;                 /* a_k (starts with floor(sqrt(D))) */
        size_t pos = 0;

        while (1) {
            /* next triple (m, d, a) of the continued‑fraction algorithm */
            m = d * a - m;
            /* use 64‑bit arithmetic to avoid overflow of m*m */
            unsigned long long m2 = (unsigned long long)m * m;
            d = (unsigned)(D - m2) / d;
            a = (16 + m) / d;
            period[pos++] = (seq_t)a;
            if (a == 32)          /* 2*a0 marks the end of the period */
                break;
        }
        period_len = pos;
    }

    if (n == 0)          /* the a0 term */
        return a0;

    /* subsequent terms cycle through the period */
    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}
