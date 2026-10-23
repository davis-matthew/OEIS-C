/*
 * A040718  Continued fraction for sqrt(746).
 * https://oeis.org/A040718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(746) – a(0) = 27, then the periodic part follows */
#include <stddef.h>

seq_t a(idx_t n)
{
    static int init = 0;
    /* period length is at most 64 for this small N */
    static seq_t period[64];
    static idx_t period_len = 0;
    const unsigned long long N = 746ULL;      /* target number */
    const unsigned long long a0 = 27ULL;      /* floor(sqrt(746)) */

    if (!init) {
        /* compute the periodic part of the continued fraction for sqrt(746) */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;
        idx_t idx = 0;
        while (1) {
            /* next triplet (m,d,a) */
            m = d * a - m;
            d = (N - (unsigned long long)m * m) / d;
            a = (a0 + m) / d;

            period[idx++] = (seq_t)a;
            if (a == 2 * a0)      /* periodic part ends when a = 2*a0 */
                break;
        }
        period_len = idx;
        init = 1;
    }

    if (n == 0)
        return (seq_t)a0;
    else {
        idx_t pos = (n - 1) % period_len;
        return period[pos];
    }
}
