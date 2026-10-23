/*
 * A144531  Triangle read by rows: T(m,n) = the smallest positive integer that
 *          has a factor in common with both m and n (2 <= n <= m).
 * https://oeis.org/A144531
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A144531: triangle read by rows.
   Entry T(m,n) (2<=n<=m) equals the smallest positive integer that
   shares a factor with both m and n.  Index n≥2 is converted to
   row m and column c via the cumulative counts
   C(m) = (m-1)*m/2.  The function uses trial division to obtain
   smallest prime factors. */

#include <stdint.h>
#include "../utils/gcd64.h"

/* integer square root of an unsigned __int128, floor value */
static unsigned long long isqrt128(unsigned __int128 v) {
    unsigned long long lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == v) return mid;
        if (mid2 < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor(sqrt(v)) */
}

/* Euclidean gcd for 64‑bit unsigned integers */

/* smallest prime factor of n (returns n if prime) */
static uint64_t spf_uint64(uint64_t n) {
    if (n <= 1) return n;
    if ((n & 1ULL) == 0) return 2;
    uint64_t lim = isqrt128((unsigned __int128)n);
    for (uint64_t p = 3; p <= lim; p += 2)
        if (n % p == 0) return p;
    return n;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;

    /* 0‑based offset in the triangle */
    idx_t k = n - 2;                     /* k >= 0 */

    /* solve m*(m-1)/2 > k  =>  m = floor((1+sqrt(1+8k))/2) + 1 */
    unsigned __int128 d = (unsigned __int128)1 + (unsigned __int128)8 * k;
    unsigned long long s = isqrt128(d);   /* floor(sqrt(d)) */
    /* ensure s is the floor sqrt (subtle when d is a perfect square) */
    if ((unsigned __int128)(s + 1) * (s + 1) <= d) s++;

    unsigned long long y = (1 + s) / 2;
    unsigned long long m = y + 1;          /* row index (>=2) */

    /* indices before row m */
    unsigned long long prev = 0;
    if (m >= 2)
        prev = ((m - 2) * (m - 1)) / 2;

    unsigned long long pos = (unsigned long long)k - prev;  /* 0‑based in row */
    unsigned long long c = pos + 2;          /* column (>=2) */

    /* compute the triangle entry */
    uint64_t g = gcd64(m, c);
    if (g > 1) {
        return (seq_t)spf_uint64(g);
    } else {
        seq_t spf_m = (seq_t)spf_uint64(m);
        seq_t spf_c = (seq_t)spf_uint64(c);
        return spf_m * spf_c;
    }
}
