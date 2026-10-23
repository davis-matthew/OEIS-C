/*
 * A187785  Number of ways to write n=x+y (x,y>=0) with {6x-1,6x+1} a twin
 *          prime pair and y a triangular number.
 * https://oeis.org/A187785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9369, u16 n <= 9369, u32 n <= 9369, u64 n <= 9369,
 *            u128 n <= 9369, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = #\{x | 0 ≤ x ≤ n, 6x-1 and 6x+1 are prime, n-x is triangular\} */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Integer square root for 64‑bit unsigned numbers (Newton’s method). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2)
        return n;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Test whether y is a triangular number: y = t(t+1)/2 for some t ≥ 0. */
static inline bool is_triangular_u64(uint64_t y)
{
    uint64_t d = 8ULL * y + 1ULL;           /* 8y + 1 */
    uint64_t s = isqrt_u64(d);
    if (s * s != d)
        return false;
    /* (s-1) must be even: s ≡ 1 (mod 2) */
    return ((s - 1) & 1) == 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x) {
        idx_t p1 = 6 * x - 1;
        idx_t p2 = 6 * x + 1;
        if (!is_prime(p1) || !is_prime(p2))
            continue;
        uint64_t y = n - x;
        if (is_triangular_u64(y))
            ++count;
    }
    return count;
}
