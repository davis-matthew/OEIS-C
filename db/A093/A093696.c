/*
 * A093696  Numbers n such that all divisors of n have an odd number of 1's in
 *          their binary expansions.
 * https://oeis.org/A093696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer such that every divisor d of n has
 *            an odd number of 1's in its binary representation
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8708 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// This implementation searches integers sequentially and checks the
// property by enumerating all divisors.  It memoises the sequence up to
// the requested index.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Recursively test all divisors of a number given its prime factorisation. */
static bool check_divisors(const uint64_t *p, const uint32_t *e,
                           int count, int idx, uint64_t current)
{
    if (idx == count) {
        /* leaf divisor */
        return (__builtin_popcountll(current) & 1U) == 1U;
    }

    uint64_t prime = p[idx];
    uint32_t exp   = e[idx];
    uint64_t val   = 1;

    for (int k = 0; k <= exp; ++k) {
        uint64_t new_val = current * val;
        if (!check_divisors(p, e, count, idx + 1, new_val))
            return false;
        if (k < exp)
            val *= prime;
    }
    return true;
}

/* Test whether integer m satisfies the sequence condition. */
static bool candidate_ok(uint64_t m, const uint64_t *primes, size_t primes_size)
{
    uint64_t n = m;
    /* factorisation */
    uint64_t factors[32];
    uint32_t exps[32];
    int fc = 0;

    for (size_t i = 0; i < primes_size && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            factors[fc] = p;
            exps[fc]   = e;
            ++fc;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        factors[fc] = n;
        exps[fc]   = 1;
        ++fc;
    }

    /* Enumerate all divisors and check parity of popcount. */
    return check_divisors(factors, exps, fc, 0, 1);
}

/* Maximum prime to sieve once at startup.
   This covers all candidates needed for about 10 000 terms. */
#define MAX_SIEVE_SIZE 1000000U

/* ----- Helper functions ------------------------------------------- */

/* ----- Sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static uint64_t *prime_list = NULL;
    static size_t   primes_size = 0;
    static uint64_t *seq       = NULL;
    static size_t   seq_size   = 0;
    static size_t   seq_cap    = 0;
    static uint64_t next_candidate = 1;

    /* initialise prime list once */
    if (!prime_list) {
        uint64_t limit = MAX_SIEVE_SIZE;
        uint8_t *is_prime = (uint8_t*)malloc(limit + 1);
        for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (uint64_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;

        /* count primes */
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

        prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        primes_size = cnt;
        size_t idx = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (is_prime[i]) prime_list[idx++] = i;

        free(is_prime);
    }

    /* initialise sequence storage */
    if (seq_cap == 0) {
        seq_cap = 128;
        seq = (uint64_t*)malloc(seq_cap * sizeof(uint64_t));
    }

    /* generate terms until we have enough */
    while (seq_size < (size_t)n) {
        while (!candidate_ok(next_candidate, prime_list, primes_size))
            ++next_candidate;

        if (seq_size >= seq_cap) {
            seq_cap *= 2;
            seq = (uint64_t*)realloc(seq, seq_cap * sizeof(uint64_t));
        }
        seq[seq_size++] = next_candidate;
        ++next_candidate;
    }

    return (seq_t)seq[n - 1];
}
