/*
 * A063640  Primes of form p*q*r + 1, where p, q and r are primes.
 * https://oeis.org/A063640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the nth prime p such that (p-1)/2 is a semiprime (i.e.
 *            (p-1)/2 = q · r for primes q, r)
 * Range:     u8 n <= 17, u16 n <= 1338, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/push_seq.h"

/* Upper bound for primes needed to reach n≈10000.  5 × 10⁶ comfortably covers
   the 10 000‑th term and keeps memory usage modest. */
#define SIEVE_LIMIT 5000000

/* Global state */
static bool        initialized = false;
static bool        *is_prime  = NULL;   /* sieve flag array */
static uint32_t    *prime_list = NULL;   /* list of primes ≤ SIEVE_LIMIT */
static size_t      prime_cnt   = 0;
static size_t      prime_cap   = 0;

static seq_t      *terms  = NULL;       /* sequence values */
static size_t      term_cnt = 0;
static size_t      term_cap = 0;
static size_t      prime_index = 0;     /* next prime to examine */

/* dynamic array helpers (C++ compilers need explicit casts) */
static void push_uint32(uint32_t **arr, size_t *cnt, size_t *cap, uint32_t val) {
    if (*cnt == *cap) {
        *cap = (*cap) ? (*cap) * 2 : 128;
        *arr = (uint32_t*)realloc(*arr, *cap * sizeof(uint32_t));
    }
    (*arr)[(*cnt)++] = val;
}

/* Build sieve and prime list */
static void init_sieve(void) {
    is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= (size_t)SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= (size_t)SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    prime_cap   = SIEVE_LIMIT / 10;
    prime_list  = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt   = 0;
    for (size_t i = 2; i <= (size_t)SIEVE_LIMIT; ++i) {
        if (is_prime[i]) push_uint32(&prime_list, &prime_cnt, &prime_cap, (uint32_t)i);
    }

    term_cap   = 256;
    terms      = (seq_t*)malloc(term_cap * sizeof(seq_t));
    term_cnt   = 0;
}

/* Check whether a small integer n (≤ SIEVE_LIMIT) is a semiprime */
static bool is_semiprime_uint64(uint64_t n) {
    if (n < 4) return false;  /* smallest semiprime = 2·2 = 4 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t q = prime_list[i];
        uint64_t q64 = (uint64_t)q;
        if (q64 * q64 > n) break;
        if (n % q == 0) {
            uint64_t r = n / q;
            if (r <= (uint64_t)SIEVE_LIMIT && is_prime[(size_t)r]) return true;
        }
    }
    return false;
}

/* Main function required by the test harness */
seq_t a(idx_t n) {
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    while (term_cnt < (size_t)n) {
        if (prime_index >= prime_cnt) break;          /* safety guard */

        uint32_t p = prime_list[prime_index++];
        if (p < 3) continue;                         /* skip the prime 2 */

        seq_t N = (p - 1) / 2;                       /* (p-1)/2 as seq_t */
        uint64_t N64 = (uint64_t)N;                  /* safe because N ≤ SIEVE_LIMIT/2 */
        if (is_semiprime_uint64(N64))
            push_seq(&terms, &term_cnt, &term_cap, (seq_t)p);
    }

    if (term_cnt < (size_t)n) return (seq_t)0;       /* fallback (should not happen) */

    return terms[(size_t)n - 1];
}
