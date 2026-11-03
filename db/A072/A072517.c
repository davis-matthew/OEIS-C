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
 * Validated: standard, pass, 51 terms through n = 9990 (2026-11-01)
 * Helpers:   ipow_i32, num_digits, triangular_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Utility includes provided by the build system  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/triangular_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* minimal m such that triangular_m(m) >= x  */
static inline idx_t m_at_least(seq_t x)
{
    /* Solve m^2 + m - 2x >= 0  ->  m = ceil((-1 + sqrt(1+8x))/2)  */
    seq_t D = x * 8 + 1;                /* 1 + 8x  */
    seq_t sqrtD = floor_sqrt(D);        /* floor sqrt */
    idx_t m = ((idx_t)sqrtD - 1) / 2;   /* floor of (-1+sqrtD)/2 */

    /* adjust upward if needed */
    if (triangular_idx(m) < x)
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
    const seq_t Tn = triangular_idx(n);
    const unsigned L = num_digits(Tn);          /* number of decimal digits of Tn */

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
        return triangular_idx(m_min);
    }
}
