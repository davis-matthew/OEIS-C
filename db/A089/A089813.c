/*
 * A089813  Expansion of Jacobi theta function (theta_2(q) -
 *          3*theta_2(q^9))/(2 q^(1/4)) in powers of q.
 * https://oeis.org/A089813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = 1 if n = m(m+1) for some integer m,
 *             -3 if n-2 = 9*m(m+1) for some integer m,
 *             a(n) = (indication of pronic) - 3*(indication of 9⋅pronic+2)
 *         That is, a(n) = [n = m(m+1)] - 3*[n-2 = 9⋅m(m+1)].
 */
#include <stdint.h>

/* Integer square root for 64‑bit values. Uses 128‑bit arithmetic to avoid overflow. */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;          /* highest possible bit of sqrt(x) */
    while (bit) {
        unsigned long long tmp = r | bit;
        __int128 sq = (__int128)tmp * tmp;
        if (sq <= x) r = tmp;
        bit >>= 1;
    }
    return r;
}

/* Helper: test whether a given integer z is of the form m(m+1) for some m >= 0. */
static inline int is_pronic_u64(unsigned long long z)
{
    unsigned long long t = 1ULL + 4ULL * z;       /* discriminant 1+4z */
    unsigned long long s = isqrt_u64(t);
    if (s * s != t) return 0;                    /* not a perfect square */
    if ((s & 1ULL) == 0) return 0;               /* square must be odd */
    unsigned long long m = (s - 1ULL) / 2ULL;
    return (m * (m + 1ULL) == z) ? 1 : 0;        /* final safety check */
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    unsigned long long nn = (unsigned long long)n;

    /* pronic part */
    res += is_pronic_u64(nn);

    /* 9⋅pronic + 2 part */
    if (nn >= 2ULL) {
        unsigned long long diff = nn - 2ULL;
        if ((diff % 9ULL) == 0ULL) {
            unsigned long long v = diff / 9ULL;
            if (is_pronic_u64(v))
                res -= 3;
        }
    }
    return res;
}
