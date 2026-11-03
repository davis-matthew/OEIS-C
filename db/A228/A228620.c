/*
 * A228620  a(n) = n - phi(n) + mu(n).
 * https://oeis.org/A228620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - phi(n) + mu(n)
 * Range:     u8 n <= 294, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 89 terms through n = 1988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;          /* defined as 0, but 0 is never passed */
    seq_t sign = 1;                /* product of (–1) for each prime factor */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;     /* not square‑free */
            }
            sign = -sign;
        }
    }
    if (tmp > 1)                   /* remaining prime factor */
        sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return tn - phi(n) + mobius(n);
}
