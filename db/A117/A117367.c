/*
 * A117367  a(n) = smallest prime greater than the smallest prime dividing n.
 * https://oeis.org/A117367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > smallest prime dividing n
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t _smallest_prime_factor_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if ((n & 1) == 0) return 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    uint64_t p = _smallest_prime_factor_uint64((uint64_t)n);
    /* p is never 0 for n > 1 */
    uint64_t candidate = p + 1;
    while (!is_prime((idx_t)candidate)) {
        ++candidate;
    }
    return (seq_t)candidate;
}
