/*
 * A040322  Continued fraction for sqrt(341).
 * https://oeis.org/A040322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Partial quotients of the continued fraction of sqrt(341)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts at n = 0 with a(0) = floor(sqrt(341)) = 18, followed by a periodic block of length 14:
   2, 6, 1, 8, 2, 1, 2, 1, 2, 8, 1, 6, 2, 36. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for the period (a0 + 14 periodic terms) */
    static seq_t period[15];
    static int initialized = 0;

    if (!initialized) {
        /* compute floor(sqrt(341)) */
        seq_t a0 = 0;
        for (uint64_t i = 0; (i + 1) * (i + 1) <= 341; ++i)
            a0 = (seq_t)(i + 1);

        period[0] = a0;

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;

        /* compute 14 terms after the initial term */
        for (int k = 1; k < 15; ++k) {
            seq_t d_prev = d;

            unsigned long long prod  = (unsigned long long)d  * (unsigned long long)a_cur;
            unsigned long long m_new = prod - (unsigned long long)m;
            m = (seq_t)m_new;

            unsigned long long num = 341ULL - m_new * m_new;
            d = (seq_t)(num / (unsigned long long)d_prev);

            a_cur = (a0 + m) / d;
            period[k] = a_cur;
        }
        initialized = 1;
    }

    if (n == 0)
        return period[0];

    size_t idx = (size_t)((n - 1) % 14);
    return period[1 + idx];
}
