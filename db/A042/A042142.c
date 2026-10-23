/*
 * A042142  Numerators of continued fraction convergents to sqrt(596).
 * https://oeis.org/A042142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   recurrence for the numerators of the CF convergents to √596. Let
 *            a0 = 24. Define p_-2 = 0, p_-1 = 1, p_n = a_n * p_{n-1} +
 *            p_{n-2} (n ≥ 0), where a_0 = a0 and a_n (n ≥ 1) are the
 *            repeating terms of the continued‑fraction expansion of √596. The
 *            first few terms are 24, 49, 122, 293, …
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Continuity fraction data for √596, fixed at compile‑time. */
    static int  initialized   = 0;
    static int  a0            = 24;      /* floor(sqrt(596)) */
    static size_t period_len   = 0;
    static int * period_terms  = NULL;

    if (!initialized) {
        /* Compute the period of the continued fraction for √596. */
        const uint64_t S = 596ULL;
        uint64_t m = 0, d = 1;
        int a = a0;

        size_t capacity = 4;
        period_terms = (int *)malloc(capacity * sizeof(int));
        if (!period_terms) abort();

        do {
            /* Standard recurrence for continued fractions of quadratic surds. */
            uint64_t m_next = d * a - m;
            uint64_t d_next = (S - m_next * m_next) / d;
            int a_next = (int)((a0 + m_next) / d_next);

            if (period_len == capacity) {
                capacity <<= 1;
                period_terms = (int *)realloc(period_terms,
                                               capacity * sizeof(int));
                if (!period_terms) abort();
            }
            period_terms[period_len++] = a_next;

            m = m_next;
            d = d_next;
            a = a_next;
        } while (a != 2 * a0);          /* terminate when a = 2*a0 (end of period) */

        initialized = 1;
    }

    /* Compute the nth numerator via the recurrence. */
    seq_t p_prev2 = 0;   /* p_-2 */
    seq_t p_prev1 = 1;   /* p_-1 */
    seq_t p_cur   = 0;   /* p_n */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? (seq_t)a0
                            : (seq_t)period_terms[(i - 1) % period_len];
        p_cur = ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return p_cur;
}
