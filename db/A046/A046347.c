/*
 * A046347  Odd composite numbers divisible by the sum of their prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A046347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   odd composite numbers divisible by the sum of their prime
 *            factors (counted with multiplicity)
 * Range:     u8 n <= 3, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64, grow_seq_ensure, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper functions from the utils library */
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/grow_seq_ensure.h"

/* Prime sieve stored as a dynamic array of uint64_t */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

/* Ensure that all primes up to 'limit' are present in the list. */
static void ensure_primes_up_to(uint64_t limit) {
    if (primes_cnt == 0) {
        grow_seq_ensure((void **)&primes, &primes_cap, 1, sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
    }
    if (limit <= primes[primes_cnt - 1]) return;
    for (uint64_t cand = primes[primes_cnt - 1] + 1; cand <= limit; ++cand) {
        if (cand > 2 && (cand & 1U) == 0) continue;          /* skip even numbers */
        bool isprime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            grow_seq_ensure((void **)&primes, &primes_cap, primes_cnt + 1, sizeof(uint64_t));
            primes[primes_cnt++] = cand;
        }
    }
}

/* Sum of prime factors (with multiplicity) for a 64‑bit integer. */
static uint64_t sum_prime_factors(uint64_t n) {
    uint64_t sum = 0;
    uint64_t temp = n;
    uint64_t limit = _floor_isqrt_u64(n);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
    }
    if (temp > 1) sum += temp;   /* remaining prime factor */
    return sum;
}

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t current_candidate = 1;

/* Main function returning the n-th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* not defined for n=0, but harmless */
    while (seq_len < (size_t)n) {
        current_candidate += 2;                      /* only odd numbers */
        if (!is_prime(current_candidate)) {           /* composite check */
            uint64_t sum = sum_prime_factors(current_candidate);
            if (sum != 0 && current_candidate % sum == 0) {
                grow_seq_ensure((void **)&seq_cache, &seq_cap, seq_len + 1, sizeof(seq_t));
                seq_cache[seq_len++] = (seq_t)current_candidate;
            }
        }
    }
    return seq_cache[n - 1];
}
