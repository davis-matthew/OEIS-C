/*
 * A122261  Characteristic function of numbers having only factors that are
 *          Pierpont primes.
 * https://oeis.org/A122261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if all prime factors of n are Pierpont primes,
 *            otherwise 0. a(1) = 1. A Pierpont prime is a prime p such that
 *            p-1 has no prime factor larger than 3
 * Range:     u8 n <= 12289, u16 n <= 12289, u32 n <= 12289, u64 n <= 12289,
 *            u128 n <= 12289, big any n
 * Validated: standard, pass, 121 terms through n = 9370 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool isPierpontPrime(uint64_t p)
{
    if (p < 2) return false;
    uint64_t rem = p - 1;
    while (rem % 2 == 0) rem /= 2;
    while (rem % 3 == 0) rem /= 3;
    return rem == 1;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1 (sequence offset). */
    if (n <= 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    while (temp > 1) {
        uint64_t factor = 0;
        /* Find smallest divisor of temp. */
        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) { factor = d; break; }
        }
        if (factor == 0) factor = temp;   /* temp is prime */

        if (!isPierpontPrime(factor)) {
            return (seq_t)0;
        }

        /* Remove all occurrences of this prime factor. */
        while (temp % factor == 0) temp /= factor;
    }
    return (seq_t)1;
}
