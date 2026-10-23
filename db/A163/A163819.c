/*
 * A163819  Expansion of (phi(q) * phi(q^10) - phi(q^2) * phi(q^5)) / 2 in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A163819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) defined multiplicatively: a(1)=1. For each prime factor p^e
 *            of n, - if p=2 or 5: factor = (-1)^e - else let m=p mod 40 if
 *            m∈{3,17,21,27,29,31,33,39}: factor = (1+(-1)^e)/2 // 0 if e odd,
 *            1 if e even if m∈{1,9,11,19}: factor = e+1 if m∈{7,13,23,37}:
 *            factor = (-1)^e * (e+1) a(n) = product of a...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Small primes sufficient for factorization of n≤10000. */
static const uint64_t SMALL_PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
    19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL,
    43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t SMALL_PRIME_COUNT =
    sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

static inline seq_t prime_power_factor(uint64_t p, int e)
{
    if (p == 2ULL || p == 5ULL)
        return (e & 1) ? (seq_t)-1 : (seq_t)1;

    uint64_t m = p % 40ULL;
    if (m == 3ULL || m == 17ULL || m == 21ULL || m == 27ULL ||
        m == 29ULL || m == 31ULL || m == 33ULL || m == 39ULL)
        return (e & 1) ? (seq_t)0 : (seq_t)1;

    if (m == 1ULL || m == 9ULL || m == 11ULL || m == 19ULL)
        return (seq_t)(e + 1);

    if (m == 7ULL || m == 13ULL || m == 23ULL || m == 37ULL)
        return (e & 1) ? -(seq_t)(e + 1) : (seq_t)(e + 1);

    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t x = (uint64_t)n;

    for (size_t i = 0; i < SMALL_PRIME_COUNT; ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (p * p > x)
            break;
        if (x % p == 0ULL) {
            int e = 0;
            while (x % p == 0ULL) {
                x /= p;
                ++e;
            }
            seq_t f = prime_power_factor(p, e);
            result *= f;
            if (result == 0)
                return result;
        }
    }

    /* Remaining prime factor, if any. */
    if (x > 1ULL) {
        seq_t f = prime_power_factor(x, 1);
        result *= f;
    }

    return result;
}
