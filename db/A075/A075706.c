/*
 * A075706  Safe primes (A005385) (p and (p-1)/2 are primes) such that 8*p+1
 *          (A023228) is also prime.
 * https://oeis.org/A075706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A075706: Safe primes p such that p and (p-1)/2 are prime and 8p+1 is prime.
 * The function computes the n-th term (1‑based) using a single prime sieve up to a
 * fixed upper bound (80 000 000) and tests each prime for the required conditions.
 * This is an exhaustive search; no recurrence or simple closed form is known.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* Returns the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_idx = 0;

    static seq_t *safe_arr = NULL;
    static size_t safe_cap = 0;
    static size_t safe_cnt = 0;

    /* One-shot initialization: build a sieve up to 80 000 000. */
    if (!initialized) {
        const uint32_t SIEVE_LIMIT = 80000000U;
        build_primes_upto(SIEVE_LIMIT, &prime_arr, &prime_cnt);
        prime_idx = 0;
        initialized = true;
    }

    /* Search until we have found at least n terms. */
    while (safe_cnt < n) {
        /* If we exhausted the prime list, expand the sieve (unlikely). */
        if (prime_idx >= prime_cnt) {
            /* Double the limit and rebuild. */
            const uint32_t NEW_LIMIT = 2 * (uint32_t)prime_cnt; /* crude upper bound */
            uint64_t *tmp_arr = NULL;
            size_t tmp_cnt = 0;
            build_primes_upto(NEW_LIMIT, &tmp_arr, &tmp_cnt);
            prime_arr = tmp_arr;
            prime_cnt = tmp_cnt;
            prime_idx = 0;
        }

        uint64_t p = prime_arr[prime_idx++];
        uint64_t q = (p - 1) / 2;               /* (p-1)/2 must be prime */
        uint64_t r = 8 * p + 1;                 /* 8p+1 must be prime */

        if (is_prime(q) && is_prime(r)) {
            grow_seq(&safe_arr, &safe_cap, safe_cnt + 1);
            safe_arr[safe_cnt++] = (seq_t)p;
        }
    }

    return safe_arr[n - 1];
}
