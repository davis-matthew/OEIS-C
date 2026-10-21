/*
 * A077770  Number of ordered pairs of integers (x,y) with n^2 < x^2 + y^2 <
 *          (n+1)^2; number of lattice points between circles of radii n and
 *          n+1.
 * https://oeis.org/A077770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051132(n+1) − A000328(n) (number of integer lattice
 *            points (x,y) with n² < x² + y² < (n+1)²)
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* NOTE: seq_t and idx_t are defined by the build system. */
/* We use 64‑bit arithmetic for intermediate values – n is at most ~10⁴. */

static inline unsigned long long isqrt_u64(unsigned long long v)
{
    /* integer square root using double, safe for v ≤ 1e10 */
    unsigned long long r = (unsigned long long) sqrt((long double)v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

static inline unsigned long long count_leq(unsigned long long L)
{
    /* number of integer y such that y² ≤ L (L ≥ 0) */
    if (L == (unsigned long long)(-1)) return 0; /* syntax safety, L never negative here */
    unsigned long long t = isqrt_u64(L);
    return 2 * t + 1;
}

seq_t a(idx_t n)
{
    unsigned long long n64 = (unsigned long long) n;
    unsigned long long nb1 = n64 + 1;
    unsigned long long nb1sq = nb1 * nb1;
    unsigned long long nsq   = n64 * n64;

    unsigned long long sum = 0;               /* total count */

    /* x = 0 term (single side, no symmetry factor) */
    unsigned long long Lh = nb1sq - 1;           // (n+1)² - 0² - 1
    unsigned long long Ll = nsq;                // n² - 0²
    unsigned long long high_c = count_leq(Lh);
    unsigned long long low_c  = count_leq(Ll);
    sum += (high_c - low_c);                    /* f(0) */

    /* x = 1 .. n (symmetry factor 2) */
    for (unsigned long long x = 1; x <= n64; ++x)
    {
        unsigned long long x2   = x * x;
        unsigned long long Lh_x = nb1sq - x2 - 1;
        unsigned long long Ll_x = nsq   - x2;

        unsigned long long high_c_x = count_leq(Lh_x);
        unsigned long long low_c_x  = count_leq(Ll_x);

        sum += 2 * (high_c_x - low_c_x);       /* f(x) counted twice */
    }

    return (seq_t) sum;
}
