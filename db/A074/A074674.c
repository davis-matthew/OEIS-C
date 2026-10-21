/*
 * A074674  Differences between successive four-digit distinct-digit primes.
 * https://oeis.org/A074674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 509, u16 n <= 509, u32 n <= 509, u64 n <= 509,
 *            u128 n <= 509, big any n
 * Helpers:   digits_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve all four‑digit primes, keep those with distinct
   decimal digits, compute the differences of each successive pair.
   a(1) is the difference between the second and the first such prime. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits_distinct.h"

#define NUM_DIGPRIMES 510
#define NUM_DIFFS (NUM_DIGPRIMES - 1)

static seq_t distinct_primes[NUM_DIGPRIMES];
static seq_t prime_diffs[NUM_DIFFS];
static size_t prime_cnt = 0;
static size_t diff_cnt = 0;
static bool initialized = false;

/* Initialise prime list and differences on the first call. */
static void init_sequence(void)
{
    if (initialized) return;

    bool is_prime[10000];
    for (size_t i = 0; i < 10000; ++i) is_prime[i] = true;
    is_prime[0] = false; is_prime[1] = false;
    for (size_t i = 2; i * i < 10000; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < 10000; j += i) {
                is_prime[j] = false;
            }
        }
    }

    for (size_t n = 1000; n < 10000; ++n) {
        if (is_prime[n] && digits_distinct(n)) {
            if (prime_cnt < NUM_DIGPRIMES) {
                distinct_primes[prime_cnt++] = (seq_t)n;
            }
        }
    }

    for (size_t i = 1; i < prime_cnt; ++i) {
        prime_diffs[i - 1] = distinct_primes[i] - distinct_primes[i - 1];
    }
    diff_cnt = prime_cnt - 1;
    initialized = true;
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    init_sequence();
    if (n < 1 || (size_t)n > diff_cnt) return 0;
    return prime_diffs[(size_t)n - 1];
}
