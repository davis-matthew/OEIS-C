/*
 * A096491  a(n) = sqrt(n) of n if n is a perfect square, otherwise a(n) =
 *          largest term in period of continued fraction expansion of square
 *          root of n.
 * https://oeis.org/A096491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sqrt(n) if n is a perfect square, otherwise
 * the largest term in the period of the continued fraction
 * expansion of sqrt(n). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t a0 = int_isqrt(n);
    if (a0 * a0 == n)          /* perfect square */
        return (seq_t)a0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_curr = a0;         /* will hold a0 in first iteration */
    seq_t max_term = 0;

    while (true) {
        /* Compute next terms of the continued‑fraction algorithm */
        idx_t m_next = (__int128)d * a_curr - m;
        idx_t d_next = (n - (__int128)m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;   /* floor division */

        if (a_next > max_term)
            max_term = a_next;

        if (a_next == 2 * a0)    /* period ends */
            break;

        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    return max_term;
}
