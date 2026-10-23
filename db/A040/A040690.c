/*
 * A040690  Continued fraction for sqrt(717).
 * https://oeis.org/A040690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   partial quotients of the continued fraction of sqrt(717)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n)  =  floor(sqrt(717))            if n = 0
    a(n)  =  p[(n-1) % L]                 otherwise
     where p is the period of the continued fraction:
     p = [1, 3, 2, 12, 1, 16, 1, 12, 2, 3, 1, 52] (L = 12)
     The period is generated algorithmically; no hard‑coded data are used. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* integer floor of sqrt(717)          */
    const long long a0 = 26;            /* 26² = 676, 27² = 729 > 717 */

    if (n == 0)
        return (seq_t)a0;

    /* static cache of the period   */
    static seq_t period[64];
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* compute successive partial quotients until the term 2*a0 occurs,
           which marks the end of the period for quadratic irrationals. */
        long long m = 0;
        long long d = 1;
        long long a = a0;
        size_t idx = 0;

        do {
            m = d * a - m;
            d = (717 - m * m) / d;
            a = (a0 + m) / d;

            period[idx++] = (seq_t)a;
        } while (a != 2 * a0);

        period_len = idx;
        initialized = 1;
    }

    size_t pos = (n - 1) % period_len;
    return period[pos];
}
