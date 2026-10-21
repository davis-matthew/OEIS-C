/*
 * A072517  Smallest triangular number beginning with the n-th triangular
 *          number other than itself.
 * https://oeis.org/A072517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the smallest triangular number > Tn that begins with the
 *            decimal representation of the n-th triangular number Tn =
 *            n*(n+1)/2, where Tn itself is excluded. The algorithm searches
 *            over increasing lengths of the candidate triangular number; for
 *            each length it computes the minimal n that yield...
 * Range:     u8 none, u16 n <= 6, u32 n <= 35, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, floor_sqrt, pow10, triangular_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Utility includes provided by the build system  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"         /* seq_t pow10(idx_t); */
#include "../utils/digit_len.h"     /* unsigned digit_len(seq_t); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t); */
#include "../utils/triangular_m.h"

/* minimal m such that triangular_m(m) >= x  */
static inline idx_t m_at_least(seq_t x)
{
    /* Solve m^2 + m - 2x >= 0  ->  m = ceil((-1 + sqrt(1+8x))/2)  */
    seq_t D = x * 8 + 1;                /* 1 + 8x  */
    seq_t sqrtD = floor_sqrt(D);        /* floor sqrt */
    idx_t m = ((idx_t)sqrtD - 1) / 2;   /* floor of (-1+sqrtD)/2 */

    /* adjust upward if needed */
    if (triangular_m(m) < x)
        m++;
    return m;
}

static inline idx_t m_at_most(seq_t high_minus1)
{
    /* maximal m with triangular_m(m) <= high_minus1 */
    return m_at_least(high_minus1) - 1;
}

seq_t a(idx_t n)
{
    /* Tn = n*(n+1)/2  */
    const seq_t Tn = triangular_m(n);
    const unsigned L = digit_len(Tn);          /* number of decimal digits of Tn */

    /* search over candidate lengths: len(Tm) = L + k, k >= 1 */
    for (idx_t k = 1; ; ++k)
    {
        const seq_t powk = pow10(k);           /* 10^k */
        const seq_t low  = Tn * powk;          /* lower bound of interval */
        const seq_t high = (Tn + 1) * powk;     /* upper bound (exclusive) */

        idx_t m_min = m_at_least(low);
        if (m_min <= n) m_min = n + 1;          /* exclude n itself   */

        idx_t m_max = m_at_most(high - 1);
        if (m_min > m_max)
            continue;                          /* no valid m for this k */

        /* The first valid m gives the smallest triangular number */
        return triangular_m(m_min);
    }
}
