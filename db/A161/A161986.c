/*
 * A161986  a(n) = k+r where k is composite(n) and r is (largest prime divisor
 *          of k) mod (smallest prime divisor of k).
 * https://oeis.org/A161986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite(n) + (largest_prime_divisor(composite(n)) mod
 *            smallest_prime_divisor(composite(n)))
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9507 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"

/* Helper: largest prime factor of a positive integer n (n≥2). */
static uint64_t _largest_prime_divisor_impl(uint64_t n)
{
    uint64_t lpf = 0;
    while ((n & 1ULL) == 0) { lpf = 2ULL; n >>= 1; }  /* divide out factors of 2 */
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL)
    {
        while (n % d == 0) {
            lpf = d;
            n /= d;
        }
    }
    if (n > 1) lpf = n;
    return lpf;
}

/* Return the nth value of OEIS A161986.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 4;                  /* first composite number */
    while (1) {
        uint64_t spf = smallest_prime_factor((uint64_t)i);
        if (spf != (uint64_t)i) {          /* i is composite */
            ++count;
            if (count == n) {
                uint64_t composite = (uint64_t)i;
                uint64_t lpf = _largest_prime_divisor_impl(composite);
                uint64_t r = lpf % spf;
                return (seq_t)(composite + r);
            }
        }
        ++i;
    }
}
