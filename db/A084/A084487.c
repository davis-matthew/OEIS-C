/*
 * A084487  Prime(n)+q, where q is the next prime such that q mod 4 = prime(n)
 *          mod 4.
 * https://oeis.org/A084487
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = Prime(n) + q, where q is the next prime such that
   q mod 4 = Prime(n) mod 4.  Offset is 2.  */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for primes, shared between calls. */
static seq_t *prime_arr  = NULL;   /* array of primes */
static size_t prime_cap = 0;       /* current capacity of prime_arr */
static size_t prime_cnt = 0;       /* number of primes stored */

/* Ensure that prime_arr has at least 'needed' entries. */
static void ensure_prime_cap(size_t needed) {
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prime_arr = tmp;
    prime_cap = new_cap;
}

/* Test whether a candidate is prime using trial division by the primes
   already computed.  The candidate is assumed to fit in seq_t. */
static bool is_prime_candidate(seq_t candidate) {
    if (candidate < 2) return false;
    if (candidate == 2) return true;
    if ((candidate & 1) == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_arr[i];
        /* Stop if p^2 > candidate. */
        seq_t p2 = p * p;
        if (p2 > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Build the prime table until it contains at least 'count' primes. */
static void build_primes_upto(size_t count) {
    if (prime_cnt >= count) return;
    ensure_prime_cap(count);
    seq_t next = prime_cnt ? prime_arr[prime_cnt - 1] + 1 : 2;
    while (prime_cnt < count) {
        if (is_prime_candidate(next))
            prime_arr[prime_cnt++] = next;
        ++next;
    }
}

/* Find the first prime at or after index 'start_idx' (0‑based) whose
   value mod 4 equals 'ref_mod'.  The search continues until such a prime
   is found, extending the prime table as necessary. */
static seq_t next_prime_same_mod4(size_t start_idx, int ref_mod) {
    size_t idx = start_idx;
    /* Ensure we have at least one prime beyond idx. */
    build_primes_upto(idx + 1);
    while (true) {
        /* Make sure we have a prime at index idx. */
        build_primes_upto(idx + 1);
        seq_t cand = prime_arr[idx];
        if ((cand & 3) == ref_mod) return cand;
        ++idx;
        /* Extending the table lazily when needed. */
    }
}

/* Public function required by the build.  n is guaranteed to be >= 2. */
seq_t a(idx_t n) {
    if (n < 2) return 0;                          /* undefined below offset */
    size_t n_idx = (size_t)n;                      /* cast to host size */
    /* Prime(n) is the (n‑1)-based element. */
    build_primes_upto(n_idx);
    seq_t prime_n = prime_arr[n_idx - 1];
    int mod = (int)(prime_n & 3);
    /* Find the next prime after prime_n with the same mod 4. */
    seq_t q = next_prime_same_mod4(n_idx, mod);    /* start from index n (0‑based) */
    return prime_n + q;
}
