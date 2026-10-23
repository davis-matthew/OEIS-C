/*
 * A040282  Continued fraction for sqrt(300).
 * https://oeis.org/A040282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) are the continued fraction coefficients of sqrt(300) (OEIS
 *            A040282). a(0) = floor(sqrt(300)) = 17, after that the period
 *            repeats 3, 8, 3, 34. The sequence is computed by the standard
 *            continued‑fraction algorithm for the square root of an integer
 * Range:     u8 n <= 0, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First term: floor(sqrt(300)) */
    static const seq_t a0 = 17;      /* because 17*17 <= 300 < 18*18 */

    if (n == (idx_t)0)
        return a0;

    /* Period of the continued fraction for sqrt(300) */
    static seq_t period[4];
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        const seq_t N = 300;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;
        size_t pos = 0;

        /* Generate period until the denominator becomes 1 again */
        while (1) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[pos++] = a_next;
            if (d_next == (seq_t)1)
                break;
            m = m_next;
            d = d_next;
            a_curr = a_next;
        }
        period_len = pos;   /* period_len == 4 for sqrt(300) */
        initialized = 1;
    }

    /* a(1) corresponds to period[0], so use (n-1) modulo period_len */
    size_t idx = ((size_t)(n - (idx_t)1)) % period_len;
    return period[idx];
}
