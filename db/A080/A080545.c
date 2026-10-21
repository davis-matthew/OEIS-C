/*
 * A080545  Characteristic function of {1} union {odd primes}: 1 if n is 1 or
 *          an odd prime, else 0.
 * https://oeis.org/A080545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n == 1 or n is an odd prime, otherwise 0
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)      return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

extern seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                     // 1 is in the set
    if ((n & 1ULL) == 0)              // even numbers (2 excluded) → 0
        return 0;
    /* n is odd and >=3 */
    return is_prime_uint64((uint64_t)n) ? 1 : 0;
}
