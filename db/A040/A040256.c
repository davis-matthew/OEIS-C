/*
 * A040256  Continued fraction for sqrt(273).
 * https://oeis.org/A040256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(273):
   a(0) = 16, thereafter the periodic block [1,1,10,1,1,32] repeats. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* We compute the continued‑fraction expansion of sqrt(273);
       the period ends when the term equals 2*a0 (32). */
    static bool init = false;
    static seq_t period[128];
    static idx_t period_len = 0;

    if (!init) {
        /* a0 = floor(sqrt(273)) = 16 */
        const uint64_t a0 = 16;
        const uint64_t D  = 273;

        uint64_t m = 0;          /* m_k */
        uint64_t d = 1;          /* d_k */
        uint64_t a_curr = a0;    /* a_k, first term is a0 (skipped) */

        /* Generate terms until the period is complete (term = 2*a0) */
        do {
            m = d * a_curr - m;
            d = (D - m * m) / d;
            a_curr = (a0 + m) / d;
            period[period_len++] = (seq_t)a_curr;
        } while (a_curr != 2 * a0);

        init = true;
    }

    if (n == 0) {
        return (seq_t)16;  /* a0 */
    }

    /* n-1 because a(1) corresponds to first period term */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
