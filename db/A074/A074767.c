/*
 * A074767  Least m > n with lpf(m) = gpf(n), where gpf(n) is the greatest
 *          prime factor of n and lpf(m) is the least prime factor of m.
 * https://oeis.org/A074767
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > n with lpf(m) = gpf(n)
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 60 (2026-11-01)
 * Helpers:   largest_prime_factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation: multiply the greatest prime factor of n   */
/*  by successive multiples until the least prime factor   */
/*  of the product equals that greatest prime factor.      */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/largest_prime_factor_u64.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; values for smaller n are
       not used in the tests.  Returning 0 for them is safe. */
    if (n < 2u)
        return 0;

    /* gpf(n): greatest prime factor of n.  The helper returns 64‑bit
       integers, so cast to idx_t. */
    idx_t gpf = (idx_t)largest_prime_factor_u64((uint64_t)n);

    /* Start with the first multiple of gpf that is strictly larger
       than n.  The formula n / gpf + 1 works because integer
       division truncates toward zero. */
    idx_t k = n / gpf + 1;

    /* Iterate over multiples of gpf until one has lpf equal to gpf. */
    while (1) {
        seq_t m = (seq_t)gpf * (seq_t)k;          /* m > n      */
        if (lpf((idx_t)m) == gpf)
            return m;
        ++k;
    }
}
