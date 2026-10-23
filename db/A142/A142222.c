/*
 * A142222  Primes congruent to 25 mod 41.
 * https://oeis.org/A142222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 25 (mod 41)
 * Range:     u8 n <= 1, u16 n <= 126, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate candidates 41·k + 25 for k ≥ 2 and test for primality,
   caching the found primes so that any order of calls runs in O(n) time. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *prime_cache = NULL;   /* array of cached primes */
static size_t prime_cap   = 0;      /* allocated capacity */
static size_t prime_len   = 0;      /* number of cached primes */
static idx_t  next_k      = 2;      /* k for next candidate 41·k + 25 */

/* Append a prime to the cache, resizing the array as needed. */
static void append_prime(seq_t val) {
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        seq_t *new_arr = alloc_seq_array(new_cap);
        /* copy existing elements */
        size_t i;
        for (i = 0; i < prime_len; ++i)
            new_arr[i] = prime_cache[i];
        if (prime_cache)
            free_seq_array(prime_cache);
        prime_cache = new_arr;
        prime_cap   = new_cap;
    }
    prime_cache[prime_len++] = val;
}

/* Ensure that at least n primes have been cached. */
static void ensure_cache(idx_t n) {
    while (prime_len < n) {
        seq_t cand = (seq_t)41 * (seq_t)next_k + (seq_t)25;
        if (is_prime_big(cand))
            append_prime(cand);
        ++next_k;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* guard against n < 1, though offset is 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
