/*
 * A115103  Primes p such that p-1 and p+1 have the same number of prime
 *          factors with multiplicity.
 * https://oeis.org/A115103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the nth prime p with ω(p-1) = ω(p+1), where ω is the
 * big‑omega function (count of prime factors with multiplicity). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega_of_uint64.h"

/* Primality test for 64‑bit integers (deterministic trial division). */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2ULL)
        return false;
    if (x % 2ULL == 0ULL)
        return x == 2ULL;
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0ULL)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) // offset is 1, but handle guard
        return 0;

    idx_t found = 0;
    for (idx_t p = 2; ; ++p) {
        if (is_prime_u64((uint64_t)p)) {
            int o1 = omega_of_uint64((uint64_t)(p - 1));
            int o2 = omega_of_uint64((uint64_t)(p + 1));
            if (o1 == o2) {
                ++found;
                if (found == n)
                    return (seq_t)p;
            }
        }
    }
}
