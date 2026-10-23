/*
 * A176705  Number of semiprimes between single (or isolated or non-twin)
 *          prime(n) and single (or isolated or non-twin) prime(n+1).
 * https://oeis.org/A176705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of semiprimes p with p > isolated_prime(n) 
 * and p < isolated_prime(n+1).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Upper bound for sieving – adequate for n up to ~10000. */
#define LIMIT 400000

static uint8_t sieve[LIMIT + 1];
static uint32_t spf[LIMIT + 1];
static uint8_t num_pf[LIMIT + 1];
static uint32_t semiprime_pref[LIMIT + 1];
static uint32_t isolated[LIMIT/2 + 1];
static size_t isolated_len = 0;
static bool initialized = false;

/* Simple sieve plus helpers.  Called once on first use. */
static void init(void)
{
    if (initialized) return;
    /* 1. Prime sieve. */
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) sieve[j] = 0;
        }
    }
    /* 2. Smallest prime factor. */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i; j <= LIMIT; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    /* 3. Number of prime factors (with multiplicity). */
    num_pf[1] = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        num_pf[i] = num_pf[i / spf[i]] + 1;
    }
    /* 4. Prefix sum of semiprimes. */
    semiprime_pref[0] = 0;
    for (size_t i = 1; i <= LIMIT; ++i) {
        semiprime_pref[i] = semiprime_pref[i - 1] + ((num_pf[i] == 2) ? 1 : 0);
    }
    /* 5. Collect isolated primes (indices start at 1). */
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (sieve[p]) {
            bool isolated_flag = true;
            if (p > 2 && sieve[p - 2]) isolated_flag = false;
            if (p + 2 <= LIMIT && sieve[p + 2]) isolated_flag = false;
            if (isolated_flag) {
                isolated[isolated_len++] = (uint32_t)p;
                if (isolated_len > LIMIT / 2) break; /* safety */
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    init();

    /* Ensure requested isolated primes exist. */
    if ((size_t)n >= isolated_len) return (seq_t)0; /* insufficient data */

    uint32_t lower = isolated[(size_t)n - 1];
    uint32_t upper = isolated[(size_t)n];

    uint32_t count = 0;
    if (upper > lower + 1) {
        count = semiprime_pref[upper - 1] - semiprime_pref[lower];
    }

    return (seq_t)count;
}
