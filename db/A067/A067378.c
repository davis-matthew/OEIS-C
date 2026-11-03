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
 * Validated: standard, pass, 68 terms through n = 9982 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, mem
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
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    results = mem_new(seq_t, cap);
    result_count = 0;
    for (idx_t val = 2; val <= LIMIT; ++val) {
        if (is_prime[val] && repr_cnt[val] >= 2) {
            if (result_count >= cap) {
                cap *= 2;
                results = mem_resize(seq_t, results, cap);
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
