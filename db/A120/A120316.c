/*
 * A120316  Consecutive refactorable numbers a(n)-1, a(n) in which 5 is the
 *          smallest prime divisor of a(n).
 * https://oeis.org/A120316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer of the form 625*k^4 such that - k is not
 *            divisible by 2 or 3 (so 5 is the smallest prime divisor of a) -
 *            a = 625*k^4 is refactorable - a-1 is refactorable
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* use the provided utility to generate a list of primes */
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static void init_primes(void) {
    if (prime_list) return;
    /* we need primes up to at least 5*max(k), 5*k ≤ 5*2000000 = 10^7
       for n up to about 10000 a safe bound is 5,000,000 */
    idx_t limit = 5000000;
    build_primes_upto(limit, &prime_list, &prime_count);
}

static seq_t *seq_cache = NULL;   /* cache of found a(n) */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t max_k_done = 0;   /* largest k processed */

/* divisor count of a 128/64-bit integer using the prime list */
static seq_t divisor_count(seq_t n) {
    seq_t tau = 1;
    seq_t rem = n;
    for (size_t i = 0; i < prime_count && (seq_t)prime_list[i] * (seq_t)prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % (seq_t)p == 0) {
            size_t exp = 0;
            while (rem % (seq_t)p == 0) {
                rem /= (seq_t)p;
                exp++;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (rem > 1) tau *= 2;   /* rem is prime */
    return tau;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n==0, return 0 */
    /* ensure primes are built */
    init_primes();

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++max_k_done;
        /* skip k divisible by 2 or 3 */
        if (max_k_done % 2 == 0 || max_k_done % 3 == 0) continue;
        uint64_t k = max_k_done;
        seq_t t = (seq_t)k;
        seq_t a_val = (seq_t)625 * t * t * t * t;  /* 625 * k^4 */

        /* check a is refactorable */
        seq_t tau_a = 1;
        uint64_t tempk = k;
        for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tempk; ++i) {
            uint64_t p = prime_list[i];
            if (tempk % p == 0) {
                size_t expk = 0;
                while (tempk % p == 0) {
                    tempk /= p;
                    expk++;
                }
                seq_t exp_a;
                if (p == 5) {
                    exp_a = (seq_t)(4 + 4 * expk);  /* 5^4 contributes 4, 5^expk contributes 4*expk */
                } else {
                    exp_a = (seq_t)(4 * expk);
                }
                tau_a *= (exp_a + 1);
            }
        }
        if (tempk > 1) {  /* remaining prime */
            uint64_t p = tempk;
            seq_t exp_a;
            if (p == 5) {
                exp_a = (seq_t)(4 + 4 * 1);
            } else {
                exp_a = (seq_t)(4 * 1);
            }
            tau_a *= (exp_a + 1);
        }
        if (a_val % tau_a != 0) continue;

        /* check (a-1) is refactorable */
        seq_t a_minus_1 = a_val - 1;
        seq_t tau_a_minus_1 = divisor_count(a_minus_1);
        if (a_minus_1 % tau_a_minus_1 != 0) continue;

        /* store a_val */
        grow_seq(&seq_cache, &cache_cap, cache_len + 1);
        seq_cache[cache_len++] = a_val;
    }

    return seq_cache[n - 1];
}
