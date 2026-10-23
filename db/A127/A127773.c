/*
 * A127773  Triangle read by rows: row n consists of n-1 zeros followed by
 *          n(n+1)/2.
 * https://oeis.org/A127773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = n if n is a triangular number n = r(r+1)/2, otherwise
 *            a(n) = 0 (OEIS A127773)
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 128‑bit values */
static idx_t isqrt128(__uint128_t x)
{
    /* rough upper bound: sqrt(8·2^64) < 2^34  */
    idx_t lo = 0;
    idx_t hi = 1ULL << 34;                  /* 1.7e10, more than enough */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;      /* upper mid */
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n is unsigned, safety */

    /* Check if n is triangular */
    __uint128_t val = (__uint128_t)n * 8 + 1;   /* 8n + 1 */
    idx_t s = isqrt128(val);

    if ((__uint128_t)s * s == val && ((s - 1) & 1ULL) == 0)
        return (seq_t)n;                    /* triangular: return n */
    else
        return (seq_t)0;                    /* otherwise zero      */
}
