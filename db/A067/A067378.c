/*
 * A067378  Primes expressible as the sum of (at least two) consecutive primes
 *          in at least 2 ways.
 * https://oeis.org/A067378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A067378 – primes that can be written as
   a sum of at least two consecutive primes in at least two distinct
   ways.  The sequence is generated once, then answers are served
   from a cached array.  The algorithm enumerates all sums of
   consecutive primes up to a fixed limit, counts the number of
   representations of every sum, and extracts those that are prime
   and have at least two representations.  The implementation
   follows the utility functions provided by the harness. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/is_prime_big.h"   /* is_prime_big – not strictly needed */

#define LIMIT 20000000UL            /* upper bound for sums of consecutive primes */
#define INITIAL_CAP 1024            /* initial capacity for the result array */

/* static state for memoisation */
static bool constructed = false;
static seq_t *results = NULL;
static idx_t result_count = 0;

/* helper: construct the sequence once */
static void build_sequence(void)
{
    /* sieve for primality test up to LIMIT */
    uint8_t *is_prime = (uint8_t*)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) return;                 /* allocation failed */
    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; (p * p) <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = 0;
        }
    }

    /* obtain the list of primes up to LIMIT */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(LIMIT, &primes, &primes_cnt);

    /* array to count representations of each sum */
    uint16_t *repr_cnt = (uint16_t*)calloc(LIMIT + 1, sizeof(uint16_t));
    if (!repr_cnt) {
        free(is_prime);
        free(primes);
        return;
    }

    /* enumerate sums of consecutive primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t sum = 0;
        for (size_t j = i; j < primes_cnt; ++j) {
            sum += (seq_t)primes[j];
            if ((j - i + 1) < 2) continue;
            if (sum > (seq_t)LIMIT) break;
            idx_t sum_idx = (idx_t)sum;
            ++repr_cnt[sum_idx];
        }
    }

    /* collect primes with at least two representations */
    size_t cap = INITIAL_CAP;
    results = (seq_t*)malloc(cap * sizeof(seq_t));
    result_count = 0;
    for (idx_t val = 2; val <= LIMIT; ++val) {
        if (is_prime[val] && repr_cnt[val] >= 2) {
            if (result_count >= cap) {
                cap *= 2;
                results = (seq_t*)realloc(results, cap * sizeof(seq_t));
            }
            results[result_count++] = (seq_t)val;
        }
    }

    /* cleanup */
    free(is_prime);
    free(primes);
    free(repr_cnt);

    constructed = true;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    if (!constructed) build_sequence();
    if (n == 0 || n > result_count) return (seq_t)0;   /* defined as 0 for out‑of‑range */
    return results[n - 1];
}
