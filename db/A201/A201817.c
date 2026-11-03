/*
 * A201817  Numbers k such that 90*k + 67 is prime.
 * https://oeis.org/A201817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 7935 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201817: numbers k such that 90*k + 67 is prime (search, no closed form) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

/* small prime trial division: eliminate obvious non‑primes */
static bool _small_prime_trial(uint64_t n)
{
    static const uint64_t primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) return n == p;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 0;
    while (true) {
        uint64_t num = 90ULL * (uint64_t)k + 67ULL;
        if (is_prime_u64(num)) {
            ++found;
            if (found == n)
                return (seq_t)k;   /* cast to the build's seq_t type */
        }
        ++k;
    }
}
