/*
 * A082844  Start with 3,2 and apply the rule a(a(1)+a(2)+...+a(n)) = a(n),
 *          fill in any undefined terms with a(t) = 2 if a(t-1) = 3 and a(t) =
 *          3 if a(t-1) = 2.
 * https://oeis.org/A082844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1+sqrt(2))*(n+2)) - floor((1+sqrt(2))*(n+1))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic: floor(k*sqrt(2)) = floor( sqrt( 2*k*k ) ) */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126; /* highest power of 4 <= x   */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* floor(k * sqrt(2))  for k >= 0  (k fits in uint64_t) */
static inline uint64_t floor_k_sqrt2(uint64_t k)
{
    unsigned __int128 val = (unsigned __int128)2 * (unsigned __int128)k * (unsigned __int128)k;
    return (uint64_t)isqrt128(val);
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to n = 1 in the problem statement */
    uint64_t k1 = (uint64_t)n + 1; /* n+1  */
    uint64_t k2 = (uint64_t)n + 2; /* n+2  */

    uint64_t s1 = floor_k_sqrt2(k1);
    uint64_t s2 = floor_k_sqrt2(k2);

    seq_t result = (seq_t)(1 + (s2 - s1));  /* 1 + difference of floor(k*sqrt2) */
    return result;
}
