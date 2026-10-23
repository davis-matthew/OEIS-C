/*
 * A190504  a(n) = n+[n*s/r]+[n*t/r]+[n*u/r]; r=golden ratio, s=r+1, t=r+2,
 *          u=r+3.
 * https://oeis.org/A190504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·φ⌋ + ⌊n·(2·φ−1)⌋ + ⌊n·(3·φ−2)⌋ where φ = (1+√5)/2
 * Range:     u8 n <= 33, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Integer square root for a 128‑bit unsigned integer.
   Returns ⌊√x⌋ as an unsigned __int128. */
static inline __uint128_t integer_sqrt_u128(__uint128_t x)
{
    unsigned long long lo = 0, hi = ULLONG_MAX, mid;
    unsigned long long res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __uint128_t mid_sq = (__uint128_t)mid * mid;
        if (mid_sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (__uint128_t)res;
}

/* Floor of m·φ, computed using only integer arithmetic. */
static inline seq_t floor_phi(seq_t m)
{
    __uint128_t m128 = (unsigned __int128)m;
    __uint128_t tmp  = m128 * m128 * 5;          /* (m·√5)² = m²·5   */
    __uint128_t s    = integer_sqrt_u128(tmp);   /* floor(m·√5)     */
    __uint128_t res  = (m128 + s) >> 1;          /* (m + floor(m√5))/2 */
    return (seq_t)res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                          /* Ensure seq_t width */
    seq_t m1 = N;                                /* n   */
    seq_t m2 = N + N;                            /* 2n  */
    seq_t m3 = N + N + N;                        /* 3n  */

    seq_t f1 = floor_phi(m1);                    /* ⌊ nφ ⌋          */
    seq_t f2 = floor_phi(m2) - N;                /* ⌊2nφ⌋ – n       */
    seq_t f3 = floor_phi(m3) - (N + N);          /* ⌊3nφ⌋ – 2n      */

    return N + f1 + f2 + f3;                     /* a(n)            */
}
