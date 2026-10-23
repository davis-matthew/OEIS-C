/*
 * A141197  a(n) = the number of divisors of n that are each one less than a
 *          power of a prime.
 * https://oeis.org/A141197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors of n that are each one less than a
 *            power of a prime. This implementation precomputes all numbers of
 *            the form p^k - 1 up to a fixed limit and then, for each n,
 *            enumerates its divisors and counts how many are in that
 *            precomputed set
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

#define MAX_LIMIT 1000000          /* maximum value to pre‑compute p^k - 1 */
static bool allowed[MAX_LIMIT + 1];  /* allowed[d] == true ⇔ d+1 is p^k for some prime p */
static bool allowed_initialized = false;

/* Build the allowed table once. */
static void initialize_allowed(void)
{
    if (allowed_initialized)
        return;
    /* Sieve to generate primes up to MAX_LIMIT+1 */
    static bool is_prime[MAX_LIMIT + 2];
    for (size_t i = 0; i <= MAX_LIMIT + 1; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= MAX_LIMIT + 1; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= MAX_LIMIT + 1; j += i)
                is_prime[j] = false;
        }
    }
    /* For each prime p generate p^k - 1 */
    for (size_t i = 2; i <= MAX_LIMIT + 1; ++i) {
        if (!is_prime[i])
            continue;
        unsigned long long prime = i;
        unsigned long long val = 1;
        /* Compute successive powers of prime */
        for (;;) {
            /* next power: val * prime */
            if (val > (unsigned long long)(-1) / prime)
                break;                /* avoid overflow */
            val *= prime;                /* = prime^k */
            if (val <= 1ULL)
                break;                  /* overflowed to 0? */
            unsigned long long d = val - 1ULL;  /* candidate divisor */
            if (d > MAX_LIMIT)
                break;
            allowed[(size_t)d] = true;
        }
    }
    allowed_initialized = true;
}

/* Count divisors of n that are in the allowed set */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    initialize_allowed();   /* build table if not already done */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_count = 0;
    factor_u64((uint64_t)n, primes, exps, &factor_count);

    /* Generate all divisors iteratively */
    static uint64_t divisors[4096];
    size_t divisors_sz = 1;
    divisors[0] = 1ULL;

    for (size_t i = 0; i < factor_count; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t old_sz = divisors_sz;
        uint64_t pow_p = 1ULL;
        for (uint64_t exp = 1; exp <= e; ++exp) {
            pow_p *= p;
            for (size_t j = 0; j < old_sz; ++j) {
                divisors[divisors_sz++] = divisors[j] * pow_p;
            }
        }
    }

    /* Count how many divisors are allowed */
    seq_t count = 0;
    for (size_t i = 0; i < divisors_sz; ++i) {
        uint64_t d = divisors[i];
        if (d <= MAX_LIMIT && allowed[(size_t)d])
            ++count;
    }
    return count;
}
