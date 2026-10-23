/*
 * A190329  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(2), t=1/s.
 * https://oeis.org/A190329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n/sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned seq_t */
static inline seq_t
isqrt_seq_t(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        if (mid == 0) {           /* safeguard against division by zero */
            low = 1;
            continue;
        }
        /* avoid overflow: mid*mid <= x  <=>  mid <= x / mid  */
        if (mid <= x / mid) {
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* conservative exit */
            high = mid - 1;
        }
    }
    return high;
}

/* Compute floor(n*sqrt(2)) as floor(sqrt(2*n*n)) */
static inline seq_t
floor_n_sqrt2(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* use 128‑bit intermediate to avoid overflow when seq_t is uint64_t */
    __uint128_t prod = (__uint128_t)2 * n * n;
    seq_t x = (seq_t)prod;
#else
    /* fallback: multiply in seq_t (may overflow for very large n,
       but the problem bounds (n <= ~10^4) keep the product safe). */
    seq_t x = (seq_t)2;
    x *= (seq_t)n;
    x *= (seq_t)n;
#endif
    return isqrt_seq_t(x);
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t t = floor_n_sqrt2(n);    /* t = floor(n*sqrt(2)) */
    seq_t t_half = t / 2;          /* floor(t/2) = floor(n/sqrt(2)) */
    return (seq_t)n + t + t_half;
}
