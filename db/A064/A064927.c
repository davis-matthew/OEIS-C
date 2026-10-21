/*
 * A064927  Period of continued fraction for sqrt(23)*n.
 * https://oeis.org/A064927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of the continued fraction of sqrt(23)*n We compute
 *            the period of the simple continued fraction for sqrt(D) with D =
 *            23 * n^2. The period length is the number of steps until the
 *            quotient a becomes 2*a0, where a0 = floor(sqrt(D))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low  = 0;
    idx_t high = n >> 1;                 /* upper bound for sqrt(n) */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                          /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* Defined only for n >= 1 */
    if (n == 0) return 0;

    idx_t D   = (idx_t)23 * n * n;       /* D = 23*n^2, fits in 64‑bit */
    idx_t a0  = isqrt_u64(D);            /* floor(sqrt(D)) */

    /* The continued fraction for sqrt(D):  m0=0, d0=1, a0 already computed */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    idx_t period = 0;
    const idx_t two_a0 = a0 << 1;        /* 2*a0 used as stop criterion */

    do {
        m = d * a - m;
        /* d = (D - m^2) / d  (d divides numerator) */
        idx_t m_sq = (unsigned __int128)m * m;
        d = (D - m_sq) / d;
        a = (a0 + m) / d;
        ++period;
    } while (a != two_a0);

    return (seq_t)period;
}
