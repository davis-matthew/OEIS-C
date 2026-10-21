/*
 * A091265  Take sequence of prime numbers (A000040) and reverse successive
 *          subsequences of lengths 1,2,3,4,...
 * https://oeis.org/A091265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091265: reverse successive blocks of the prime sequence
   a(n) = prime at position (t(n) - (n - (t(n-1)+1))) where
   t(k) = k*(k+1)/2 is the triangular numbers.
   This implementation generates primes on demand and
   reuses a static list for efficiency. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static prime storage */
static uint64_t *prime_arr = NULL;      /* array of primes */
static size_t    prime_cnt = 0;         /* number of primes stored */
static size_t    prime_cap = 0;         /* capacity of prime_arr */

/* Grow prime array to at least new_cap capacity */
static void grow_prime_array(size_t new_cap) {
    if (new_cap <= prime_cap) return;
    size_t cap = prime_cap ? prime_cap * 2 : new_cap;
    if (cap < new_cap) cap = new_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE); /* simple error handling */
    prime_arr = tmp;
    prime_cap = cap;
}

/* Check primality of cand using stored primes as trial divisors */
static bool is_prime_candidate(uint64_t cand) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Append cand to prime array */
static void add_prime(uint64_t cand) {
    grow_prime_array(prime_cnt + 1);
    prime_arr[prime_cnt++] = cand;
}

/* Ensure at least 'need' primes are stored (indices 1..need) */
static void ensure_primes_upto(size_t need) {
    if (prime_cnt >= need) return;
    uint64_t cand = (prime_cnt == 0) ? 2 : (prime_arr[prime_cnt - 1] + 1);
    while (prime_cnt < need) {
        if (cand < 2) cand = 2;
        if (is_prime_candidate(cand)) add_prime(cand);
        ++cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Find block k such that sum_{i=1}^k i >= n */
    idx_t k = 1;
    idx_t sum = 1;               /* triangular number s(k) */
    while (sum < n) {
        ++k;
        sum += k;                /* sum becomes s(k) */
    }

    idx_t prev_sum = sum - k;    /* s(k-1) */
    idx_t pos = n - (prev_sum + 1); /* 0-based offset inside block */
    idx_t prime_idx = sum - pos;    /* 1-based index of prime in standard order */

    ensure_primes_upto((size_t)prime_idx);

    return (seq_t)prime_arr[prime_idx - 1];
}
