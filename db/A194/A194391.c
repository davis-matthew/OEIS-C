/*
 * A194391  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(12) and < > denotes fractional part.
 * https://oeis.org/A194391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that Sum_{k=1..m} (
 *            {k*r+1/2} - {k*r} ) > 0, where r = sqrt(12). This is equivalent
 *            to 2 * # {k ≤ m : frac(k*r) < 1/2} > m
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for unsigned __int128 (floor(sqrt(x))) */
static inline uint64_t isqrt_u128(unsigned __int128 x) {
    uint64_t low = 0;
    uint64_t high = (uint64_t)(-1);          /* ULLONG_MAX */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (__uint128_t)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* test whether frac(k*r) < 1/2, with r = sqrt(12) */
static inline bool frac_lt_half(uint64_t k) {
    unsigned __int128 square = (__uint128_t)12 * k * k;
    uint64_t n = isqrt_u128(square);                 /* n = floor(sqrt(12*k^2)) */
    unsigned __int128 lhs = (__uint128_t)48 * k * k;  /* 48*k^2 */
    unsigned __int128 t   = (__uint128_t)n * 2 + 1;   /* 2*n+1 */
    unsigned __int128 rhs = t * t;                    /* (2*n+1)^2 */
    return lhs < rhs;                                /* < 0.5 comparison */
}

/* sequence generator */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                    /* safety for 0-index */
    uint64_t count = 0;                             /* how many terms found */
    uint64_t c     = 0;                             /* #k with frac(k*r) < 1/2 */
    for (uint64_t m = 1; ; ++m) {
        if (frac_lt_half(m)) ++c;
        if ((unsigned __int128)c * 2 > m) {         /* 2*c > m  -> m is a term */
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
