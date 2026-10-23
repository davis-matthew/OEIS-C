/*
 * A008836  Liouville's function lambda(n) = (-1)^k, where k is number of
 *          primes dividing n (counted with multiplicity).
 * https://oeis.org/A008836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) = (-1)^bigomega(n) where bigomega(n) is the count of prime
 *            factors of n with multiplicity (Liouville's lambda function)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached list of primes up to MAX_PRIME. */
#define MAX_PRIME 1000000   /* adequate for n up to ~10^12 */
static uint32_t g_primes[80000];
static size_t g_prime_count = 0;
static bool g_primes_ready = false;

/* Sieve of Eratosthenes to fill g_primes. */
static void init_primes(void)
{
    if (g_primes_ready) return;
    static bool is_composite[MAX_PRIME + 1] = { false };
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            if (g_prime_count < sizeof(g_primes)/sizeof(g_primes[0]))
                g_primes[g_prime_count++] = i;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    is_composite[j] = true;
            }
        }
    }
    g_primes_ready = true;
}

/* Return Liouville λ(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    init_primes();

    uint64_t m = (uint64_t)n;
    int parity = 0;   /* parity of Ω(n) */

    for (size_t i = 0; i < g_prime_count; ++i) {
        uint32_t p = g_primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > m) break;

        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        parity ^= (exp & 1);
    }

    /* If something remains, it is a prime > sqrt(original n). */
    if (m > 1)
        parity ^= 1;   /* exponent is 1 */

    return (parity == 0) ? (seq_t)1 : (seq_t)-1;
}
