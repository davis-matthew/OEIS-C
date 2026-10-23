/*
 * A161841  Number of factors, with repetition, in all distinct pairs (a <= b)
 *          such that a*b = n.
 * https://oeis.org/A161841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = d(n) + [n is a perfect square]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) = number of divisors of n. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    if (x == 0) return 0;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Divisor count for a 64‑bit integer. */
static inline uint64_t divisor_count(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t res = 1;
    uint64_t exp = 0;

    while ((n & 1) == 0) {
        exp++;
        n >>= 1;
    }
    if (exp) res *= (exp + 1);

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            do {
                n /= p;
                exp++;
            } while (n % p == 0);
            res *= (exp + 1);
        }
    }
    if (n > 1) res *= 2;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based, but our formula works for all n >= 1. */
    uint64_t d = divisor_count((uint64_t)n);
    idx_t sqrt_n = isqrt_u64(n);
    bool is_sq = (sqrt_n * sqrt_n == n);
    uint64_t result = d + (is_sq ? 1u : 0u);
    return (seq_t)result;
}
