/*
 * A050870  T(h,k) = binomial(h,k) - A050186(h,k).
 * https://oeis.org/A050870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(h,k) = binomial(h,k) - A050186(h,k) where h,k are
 *            obtained from n via the anti‑diagonal enumeration. A050186(h,k)
 *            = sum_{d | gcd(h,k)} μ(d) * binomial(h/d, k/d)
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binomial.h"

/* gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function μ(n), n >= 1 */
static int mobius_uint64(uint64_t n)
{
    if (n == 1) return 1;
    int cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            n /= p;
            if (n % p == 0) return 0;   /* squared prime factor */
            ++cnt;
        }
    }
    if (n > 1) ++cnt;   /* remaining prime */
    return (cnt & 1) ? -1 : 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition, directly handle */
    if (n == 0) return (seq_t)0;

    /* Determine (h,k) corresponding to index n */
    /* h is the largest integer with h*(h+1)/2 <= n */
    idx_t h = (idx_t)((sqrt((long double)1 + 8.0L * n) - 1) / 2);
    /* adjust possible rounding errors */
    while ( ((unsigned __int128)(h + 1) * (h + 2) / 2) <= n) ++h;
    while ( ((unsigned __int128)h * (h + 1) / 2) > n) --h;

    idx_t offset = (idx_t)(((unsigned __int128)h * (h + 1)) / 2);
    idx_t k = n - offset;

    seq_t bin_total = binomial(h, k);

    /* If h is 0 we are already at the special case a(0) */
    /* but for completeness handle h==0 gracefully */
    if (h == 0) return (seq_t)0;

    /* Compute A050186(h,k) = sum_{d | gcd(h,k)} μ(d) * binomial(h/d, k/d) */
    idx_t g = gcd_idx(h, k);
    __int128 gsum = 0;   /* use signed intermediate to handle subtraction */

    /* Enumerate all divisors of g */
    for (idx_t d = 1; d * d <= g; ++d)
    {
        if (g % d != 0) continue;

        /* Process divisor d */
        int mu = mobius_uint64(d);
        if (mu != 0)
        {
            seq_t term = binomial(h / d, k / d);
            if (mu > 0)
                gsum += ( __int128 )term;
            else
                gsum -= ( __int128 )term;
        }

        /* If d != g/d handle pair divisor */
        idx_t d2 = g / d;
        if (d2 != d)
        {
            int mu2 = mobius_uint64(d2);
            if (mu2 != 0)
            {
                seq_t term2 = binomial(h / d2, k / d2);
                if (mu2 > 0)
                    gsum += ( __int128 )term2;
                else
                    gsum -= ( __int128 )term2;
            }
        }
    }

    seq_t aperiodic = (seq_t)gsum;   /* gsum is guaranteed >= 0 */
    seq_t periodic = bin_total - aperiodic;
    return periodic;
}
