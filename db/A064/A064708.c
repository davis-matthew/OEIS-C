/*
 * A064708  Initial term of run of (at least) n consecutive numbers with just
 *          2 distinct prime factors.
 * https://oeis.org/A064708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching runs of consecutive numbers with exactly
   two distinct prime factors. */

/* ----------------------------------------------------
 * Helper: generate small primes up to a fixed limit.
 * ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_LIMIT 50000
#define MAX_N      10000  /* maximum index we support */

static uint32_t small_primes[6000];
static size_t prime_cnt = 0;

/* simple sieve of Eratosthenes */
static void build_small_primes(void) {
    bool is_composite[PRIME_LIMIT + 1] = {false};
    size_t i, j;
    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < 6000) small_primes[prime_cnt++] = (uint32_t)i;
            for (j = i * 2; j <= PRIME_LIMIT; j += i) is_composite[j] = true;
        }
    }
}

/* ----------------------------------------------------
 * Test if x has exactly two distinct prime factors.
 * ---------------------------------------------------- */
static bool two_distinct_primes_uint64(uint64_t x) {
    uint8_t distinct = 0;
    size_t i;
    for (i = 0; i < prime_cnt && ((uint64_t)small_primes[i] *
                                  (uint64_t)small_primes[i]) <= x; ++i) {
        uint32_t p = small_primes[i];
        if (x % p == 0) {
            ++distinct;
            while (x % p == 0) x /= p;
            if (distinct > 2) return false;
        }
    }
    if (x > 1) ++distinct; /* remaining prime factor */
    return distinct == 2;
}

/* ----------------------------------------------------
 * Cache the computed a(n) values.
 * ---------------------------------------------------- */
static seq_t cached_a[MAX_N + 1];
static bool initialized = false;

/* ----------------------------------------------------
 * Main function required by the problem.
 * ---------------------------------------------------- */
seq_t a(idx_t n) {
    /* lazy initialization of cache */
    if (!initialized) {
        size_t i;
        for (i = 0; i <= MAX_N; ++i) cached_a[i] = 0;

        /* build primes for factorization */
        build_small_primes();

        /* search up to a reasonable limit */
        const idx_t LIMIT = 1000000; /* sufficiently large to find runs up to length 8 */
        idx_t cur_start = 0; /* start of current run */
        idx_t cur_len   = 0; /* current run length */

        size_t found = 0; /* number of a(k) values found up to MAX_N */

        for (i = 2; i <= LIMIT; ++i) {
            bool is2dp = two_distinct_primes_uint64((uint64_t)i);
            if (is2dp) {
                if (cur_len == 0) cur_start = i;
                ++cur_len;
            } else {
                if (cur_len > 0) {
                    /* we have a run of length cur_len starting at cur_start */
                    idx_t len;
                    for (len = 1; len <= cur_len; ++len) {
                        if (len <= MAX_N && cached_a[len] == 0) {
                            cached_a[len] = (seq_t)cur_start;
                            ++found;
                        }
                    }
                    cur_len = 0;
                }
            }
            /* if we already found all values up to MAX_N, we could stop,
               but we want to settle at least for n <= 8 and rely that
               higher indices will stay zero.               */
            if (found >= 8) break; /* enough for known terms */
        }
        /* handle the last run if it reached the limit */
        if (cur_len > 0 && found < 8) {
            idx_t len;
            for (len = 1; len <= cur_len; ++len) {
                if (len <= MAX_N && cached_a[len] == 0) {
                    cached_a[len] = (seq_t)cur_start;
                    ++found;
                }
            }
        }
        initialized = true;
    }

    if (n <= MAX_N) return cached_a[(idx_t)n];
    return (seq_t)0; /* undefined for n > MAX_N (sequence has no more terms) */
}
