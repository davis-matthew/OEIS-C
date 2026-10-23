/*
 * A134567  a(n) = least m such that {-m*tau} < {n*tau}, where { } denotes
 *          fractional part and tau = (1 + sqrt(5))/2.
 * https://oeis.org/A134567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that {-m*tau} < {n*tau} (tau = (1+√5)/2)
 * Range:     u8 n <= 88, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 63;          /* start with the highest bit */
    while (bit > 0) {
        unsigned long long t = res + bit;
        if (t <= x / t)           /* avoid overflow: t*t <= x  ⇔  t <= x/t  */
            res = t;
        bit >>= 1;
    }
    return res;
}

/* Exact floor of k*tau where tau = (1+√5)/2 */
static inline seq_t floor_phi(idx_t k)
{
    unsigned long long kk = (unsigned long long)k;
    unsigned long long t  = 5ULL * kk * kk;        /* 5*k^2 fits in 64 bits for our limits */
    unsigned long long s  = isqrt_u64(t);          /* floor(sqrt(5*k^2)) = floor(k*√5)  */
    unsigned long long r  = (kk + s) >> 1;         /* (k + floor(k*√5)) / 2 */
    return (seq_t)r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t fn = floor_phi(n);          /* floor(n*tau) */

    for (idx_t m = 1; ; ++m) {
        seq_t fm   = floor_phi(m);           /* floor(m*tau)   */
        seq_t fmn  = floor_phi(m + n);       /* floor((m+n)*tau) */

        if (fmn > fm + fn)                  /* carry occurs ⇒ sum of fractional parts > 1 */
            return (seq_t)m;
    }
}
