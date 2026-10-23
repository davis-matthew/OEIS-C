/*
 * A040959  Continued fraction for sqrt(991).
 * https://oeis.org/A040959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the n-th term of the continued fraction expansion of
 *            sqrt(991). a(0) = 31; for n > 0 the sequence is periodic with
 *            period computed by the standard continued‐fraction algorithm for
 *            square roots
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Periodic part of the continued fraction (excluding the integer part). */
    static bool initialized = false;
    static seq_t period[200];
    static size_t period_len = 0;

    if (!initialized) {
        const seq_t N = 991;          /* the square‐free integer */
        const seq_t a0 = 31;          /* floor(sqrt(991)) */
        seq_t m = 0;                  /* m0 */
        seq_t d = 1;                  /* d0 */
        seq_t a_curr = a0;            /* a0 */
        size_t i = 0;

        while (1) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[i++] = a_next;
            if (a_next == a0 * 2)   /* period ends with 2*a0 */
                break;

            m = m_next;
            d = d_next;
            a_curr = a_next;
        }
        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return 31;

    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}
