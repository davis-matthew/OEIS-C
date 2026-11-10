/*
 * A078357  Minimal positive solution x of Pell equation y^2 - A077426(n)*x^2
 *          = -4.
 * https://oeis.org/A078357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive x with y^2 - A077426(n)*x^2 = -4;
 *            computed by the continued fraction of (1+sqrt(D))/2 (search)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 46, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: standard, pass, 74 terms through n = 119 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t A078357_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static inline bool A077426_odd_period(idx_t D)
{
    idx_t a0 = A078357_isqrt(D);
    if (a0 * a0 == D)
        return false;
    idx_t m = 0, d = 1, a = a0;
    bool odd = false;
    while (1)
    {
        m = (idx_t)((unsigned __int128)d * a - m);
        d = (D - (idx_t)((unsigned __int128)m * m)) / d;
        a = (a0 + m) / d;
        odd = !odd;
        if (a == 2 * a0)
            break;
    }
    return odd;
}

static idx_t A077426_term(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n)
    {
        ++k;
        if ((k & 1) == 0)
            continue;
        if (!A077426_odd_period(k))
            continue;
        ++count;
    }
    return k;
}

seq_t a(idx_t n)
{
    idx_t D = A077426_term(n);
    idx_t s = A078357_isqrt(D);
    idx_t P = 1, Q = 2;
    seq_t pm2 = 0, pm1 = 1, qm2 = 1, qm1 = 0;
    unsigned long long k = 0;
    while (1)
    {
        idx_t aa = (P + s) / Q;
        seq_t p = (seq_t)aa * pm1 + pm2;
        seq_t q = (seq_t)aa * qm1 + qm2;
        idx_t P2 = aa * Q - P;
        idx_t Q2 = (D - P2 * P2) / Q;
        if ((k & 1ULL) == 0 && Q2 == 2)
            return q;
        pm2 = pm1; pm1 = p;
        qm2 = qm1; qm1 = q;
        P = P2; Q = Q2;
        k++;
    }
}
