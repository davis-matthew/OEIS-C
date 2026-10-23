/*
 * A041956  Numerators of continued fraction convergents to sqrt(501).
 * https://oeis.org/A041956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=22, a(1)=45,
   a(n) = c[n] * a(n-1) + a(n-2)   (n≥2),
   where {c[n]} are the continued‑fraction coefficients of √501
   (periodic with length 28). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data for the continued‑fraction period of √501 */
    static bool initialized = false;
    static uint32_t *period   = NULL;
    static size_t   period_len = 0;

    const idx_t D   = 501;
    const idx_t a0  = 22;        /* floor(√501) */

    /* Compute the period once */
    if (!initialized) {
        /* Period will be at most a few dozen entries; allocate a bit more. */
        size_t cap = 32;
        uint32_t *tmp = (uint32_t *)malloc(cap * sizeof(uint32_t));
        size_t pos = 0;

        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        idx_t a_next;        /* declared outside to be usable in while condition */
        do {
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            a_next = (a0 + m_next) / d_next;

            if (pos == cap) {
                cap <<= 1;
                tmp = (uint32_t *)realloc(tmp, cap * sizeof(uint32_t));
            }
            tmp[pos++] = (uint32_t)a_next;

            m = m_next;
            d = d_next;
            a = a_next;
        } while (a_next != 2 * a0);   /* period ends when a_k = 2*a0 */

        period = (uint32_t *)malloc(pos * sizeof(uint32_t));
        for (size_t i = 0; i < pos; ++i)
            period[i] = tmp[i];
        period_len = pos;
        free(tmp);
        initialized = true;
    }

    /* Compute the n‑th numerator via the recurrence */
    seq_t p_nm2 = 0;    /* p_{-2} */
    seq_t p_nm1 = 1;    /* p_{-1} */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = a0;
        } else {
            size_t idx = (size_t)(i - 1) % period_len;
            ai = (seq_t)period[idx];
        }
        seq_t p_i = ai * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_i;
    }
    return p_nm1;
}
