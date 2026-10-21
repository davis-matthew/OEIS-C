/*
 * A225223  Primes of the form p - 1, where p is a practical number (A005153).
 * https://oeis.org/A225223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 2181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the n-th prime p such that p+1 is a practical
   number (A005153).  The implementation enumerates integers, tests
   practicality and primality, and memoises the found primes.
   The first several terms are: 3,5,7,11,17,19,23,29,31,41,...
   The algorithm runs in O(m) where m is the largest integer examined
   to find the n-th term. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* ---------- dynamic storage for the terms ---------- */
static seq_t *term_cache = NULL;  /* a(1), a(2), ... */
static size_t term_count = 0;     /* number of terms cached */
static size_t term_cap = 0;       /* allocated capacity */

/* Ensure term_cache can hold at least 'needed' terms */
static void ensure_terms(size_t needed) {
    if (needed <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (term_cache) {
        memcpy(new_mem, term_cache, term_count * sizeof(seq_t));
        free(term_cache);
    }
    term_cache = new_mem;
    term_cap = new_cap;
}

/* ---------- prime list for factorisation ---------- */
static uint64_t *prime_list = NULL;     /* list of primes up to prime_limit */
static size_t   prime_count = 0;
static uint64_t prime_limit = 0;        /* maximum prime generated */

/* Build or extend prime_list to cover all primes up to 'limit' */
static void ensure_primes(uint64_t limit) {
    if (limit <= prime_limit) return;
    uint64_t *new_ptr = NULL;
    /* build_primes_upto fills an array of uint64_t */
    build_primes_upto(limit, (uint64_t **)&new_ptr, &prime_count);
    free(prime_list);
    prime_list = new_ptr;
    prime_limit = limit;
}

/* ---------- test for practical number (A005153) ---------- */
static bool is_practical(uint64_t n) {
    if (n == 1) return true;

    uint64_t temp = n;
    unsigned __int128 sum = 1;   /* sigma of already processed part */

    ensure_primes(_floor_isqrt_u64(n) + 1U);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            if ((unsigned __int128)p > (unsigned __int128)1 + sum) return false;

            /* compute sigma(p^exp) = (p^(exp+1)-1)/(p-1) */
            unsigned __int128 pow_pa1 = 1;
            for (int j = 0; j < exp + 1; ++j) pow_pa1 *= p;
            unsigned __int128 sigma_pa = (pow_pa1 - 1) / (p - 1);
            sum *= sigma_pa;
        }
    }

    if (temp > 1) {   /* remaining prime factor, exponent = 1 */
        uint64_t p = temp;
        if ((unsigned __int128)p > (unsigned __int128)1 + sum) return false;
        unsigned __int128 sigma_pa = 1 + p;
        sum *= sigma_pa;
    }

    return true;
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* the sequence is defined for n >= 1 */

    static uint64_t last_checked = 1;   /* largest integer examined so far */

    /* generate terms until we have at least n terms */
    while (term_count < (size_t)n) {
        ++last_checked;
        if (is_practical(last_checked)) {
            if (is_prime(last_checked - 1)) {
                ensure_terms(term_count + 1);
                term_cache[term_count++] = (seq_t)(last_checked - 1);
            }
        }
    }
    return term_cache[n - 1];
}
