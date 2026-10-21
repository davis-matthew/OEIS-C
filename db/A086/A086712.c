/*
 * A086712  Number of times the n-th prime power can be written as an
 *          arithmetic mean of two other prime powers.
 * https://oeis.org/A086712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 893, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   upper_bound_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation: For the n‑th prime power v (counting the number 1 as a
 * prime power), a(n) equals the number of unordered pairs of distinct
 * prime powers (excluding v itself) whose arithmetic mean is v.  In other
 * words we count the solutions (p_i , p_j), p_i < p_j, in primepowers
 * such that p_i + p_j = 2*v.
 *
 * The implementation precomputes all prime powers up to a fixed limit
 * (covering far more than 10000 terms) once and reuses the list for
 * subsequent calls.  The list is sorted, and a lookup table of size
 * LIMIT+1 marks which integers are prime powers.  The size of INT_LIMIT
 * is chosen large enough to guarantee that the 10000‑th prime power
 * (including 1) is well below it, and that the corresponding arithmetic
 * sum 2*v also stays within the table.
 *
 * This method satisfies the time constraint (<2s for n up to ~10000)
 * and works for all builds that support unsigned integer arithmetic
 * up to 128 bits.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/upper_bound_uint64.h"

/*
 * Arbitrary-precision or 64‑bit builds supply their own seq_t and idx_t.
 * The implementation relies only on basic C arithmetic and the
 * standard library.
 */

/* Upper bound for the largest prime power considered. */
#define LIMIT        500000UL   /* must be > 2 * value of the 10000‑th prime power */

/* Upper bound for the number of prime powers that will be stored. */
#define MAX_PRIMEPOW 300000

/* One‑time initialized data. */
static bool          initialized = false;
static uint64_t      primes[MAX_PRIMEPOW];
static size_t        primes_cnt  = 0;
static uint64_t      primepowers[MAX_PRIMEPOW];
static size_t        primepowers_cnt = 0;
static uint8_t       is_prime_power[LIMIT + 1];

/* Comparator for qsort on uint64_t. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* One‑time initialization: sieve, primes, prime powers, lookup table. */
static void init(void)
{
    /* Sieve for primes up to LIMIT. */
    static uint8_t is_prime[MAX_PRIMEPOW + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Extract primes. */
    primes_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;

    /* Generate prime powers (including 1). */
    primepowers_cnt = 0;
    /* Add 1 explicitly. */
    if (primepowers_cnt < MAX_PRIMEPOW)
        primepowers[primepowers_cnt++] = 1ULL;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        uint64_t val = p;
        while (primepowers_cnt < MAX_PRIMEPOW && val <= LIMIT) {
            primepowers[primepowers_cnt++] = val;
            if (val > LIMIT / p) break;
            val *= p;
        }
    }

    /* Sort the prime powers. */
    qsort(primepowers, primepowers_cnt, sizeof(uint64_t), cmp_uint64);

    /* Build lookup table. */
    memset(is_prime_power, 0, sizeof(is_prime_power));
    for (size_t i = 0; i < primepowers_cnt; ++i) {
        uint64_t v = primepowers[i];
        if (v <= LIMIT)
            is_prime_power[(size_t)v] = 1;
    }

    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (n == 0 || (idx_t)primepowers_cnt < n)
        return 0;   /* out of bounds or invalid n */

    uint64_t v = primepowers[n - 1u];

    /* Find the first index of a prime power greater than v. */
    size_t up = upper_bound_uint64(primepowers, primepowers_cnt, v);

    seq_t count = 0;
    for (size_t i = 0; i < up; ++i) {
        uint64_t a_val = primepowers[i];
        if (a_val == v)   /* skip pair where a equals target */
            continue;
        uint64_t b_val = 2ULL * v - a_val;
        if (b_val <= LIMIT && b_val > a_val && is_prime_power[(size_t)b_val])
            ++count;
    }

    return count;
}
