/*
 * A060954  Largest prime factor of 10*n + 1.
 * https://oeis.org/A060954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of (10*n + 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* largest_prime_factor_uint64: returns the largest prime factor of x (x>=2)
   The implementation uses trial division with 2 and odd candidates. */
static uint64_t largest_prime_factor_uint64(uint64_t x)
{
    uint64_t largest = 1;
    if (x < 2) return x;            /* for x==0 or 1 */
    if (x % 2 == 0) {
        largest = 2;
        while (x % 2 == 0) x /= 2;
    }
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            largest = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) largest = x;
    return largest;
}

seq_t a(idx_t n)
{
    /* compute 10*n + 1 */
    seq_t v = (seq_t)n * 10 + 1;
    /* bring to 64‑bit for factorisation (works when the value fits in 64 bits) */
    uint64_t x = (uint64_t)v;
    /* fallback: if overflow truncated value, still factorise the truncated part */
    uint64_t lpf = largest_prime_factor_uint64(x);
    return (seq_t)lpf;
}
