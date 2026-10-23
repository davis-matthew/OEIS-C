/*
 * A010164  Continued fraction for sqrt(93).
 * https://oeis.org/A010164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = ⌊√93⌋ = 9. For n > 0 the coefficients are the periodic
 *            continued‑fraction terms of √93, repeating with period
 *            [1,1,1,4,6,4,1,1,1,18]
 * Range:     u8 n <= 9207, u16 n <= 9207, u32 n <= 9207, u64 n <= 9207,
 *            u128 n <= 9207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for one‑time initialization. */
    static bool init = false;
    static seq_t a0;          /* ⌊√93⌋ */
    static seq_t period[32];  /* periodic block */
    static idx_t period_len = 0;

    if (!init) {
        /* ---- Compute ⌊√93⌋ ---- */
        idx_t D = 93;
        idx_t s = 0;
        while ((s + 1) * (s + 1) <= D) {
            ++s;
        }
        a0 = (seq_t)s;          /* a0 = 9 */

        /* ---- Generate the period using the standard algorithm ---- */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = s;            /* current ‘a’ */
        idx_t idx = 0;

        while (1) {
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (s + m_next) / d_next;

            period[idx++] = (seq_t)a_next;

            if (a_next == 2 * s)     /* period finished */
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }

        period_len = idx;
        init = true;
    }

    if (n == 0)             /* the initial term */
        return a0;

    /* Following terms are periodic */
    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
