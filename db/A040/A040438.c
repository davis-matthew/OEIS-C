/*
 * A040438  Continued fraction for sqrt(460).
 * https://oeis.org/A040438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction coefficients of sqrt(460)
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(460)), the remaining coefficients repeat with period 12. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static data used to compute the period only once. */
    static bool init = false;
    static seq_t period[256];
    static idx_t period_len = 0;
    static seq_t a0 = 0;

    if (!init) {
        /* The square‑free part of the continued fraction. */
        const unsigned long long D = 460ULL;

        /* a0 = floor(sqrt(460)) */
        a0 = floor_sqrt((seq_t)D);

        /* Variables of the standard continued fraction recurrence. */
        unsigned long long m  = 0;          /* m0 */
        unsigned long long d  = 1;          /* d0 */
        unsigned long long a_i = (unsigned long long)a0;  /* a0 */

        idx_t i = 0;
        while (1) {
            /* m_k   = d_{k-1} * a_{k-1} - m_{k-1}   */
            m = d * a_i - m;
            /* d_k   = (D - m_k²) / d_{k-1}           */
            d = (D - m * m) / d;
            /* a_k   = floor((a0 + m_k) / d_k)       */
            a_i = ( (unsigned long long)a0 + m ) / d;

            /* Store the coefficient in the period array. */
            period[i++] = (seq_t)a_i;

            /* The period ends when a_k == 2*a0. */
            if (a_i == a0 * 2ULL) {
                period_len = i;
                break;
            }
        }

        init = true;
    }

    if (n == 0ULL)
        return a0;

    /* Index into the periodic part. */
    idx_t pos = (n - 1ULL) % period_len;
    return period[pos];
}
