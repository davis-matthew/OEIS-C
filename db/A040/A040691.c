/*
 * A040691  Continued fraction for sqrt(718).
 * https://oeis.org/A040691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√718⌋, * a(n) for n>0 are the continued‑fraction
 *            coefficients * of √718, repeated periodically
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Period of continued fraction for sqrt(718) is small (< 60). */
    static bool init = false;
    static seq_t period[64];
    static idx_t period_len = 0;
    static seq_t a0 = 0; /* integer part of sqrt(718) */

    if (!init) {
        /* --- compute a0 = floor(sqrt(718)) --- */
        uint64_t s = 0;
        while ((s + 1) * (s + 1) <= 718) ++s;
        a0 = (seq_t)s;

        /* --- compute period using standard algorithm --- */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_prev = s;  /* a0 */
        int i = 0;
        while (1) {
            uint64_t m_next = d * a_prev - m;
            uint64_t d_next = (718 - m_next * m_next) / d;
            uint64_t a_next = (s + m_next) / d_next;
            period[i++] = (seq_t)a_next;
            if (a_next == 2 * s) break;   /* period ends when a = 2*a0 */
            m = m_next;
            d = d_next;
            a_prev = a_next;
        }
        period_len = (idx_t)i;
        init = true;
    }

    if (n == 0) return a0;
    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
