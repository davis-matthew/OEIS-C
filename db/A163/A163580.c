/*
 * A163580  Primes of the form floor(k+A000217(k-1)*Pi), Pi = A000796, k
 *          integer.
 * https://oeis.org/A163580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form floor(k + T(k-1)*π), where T(k-1) = (k-1)*k/2
 *            and π = 3.141592653589793238462643383279502884 The function
 *            a(idx_t n) returns the n-th prime in this sequence (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL; /* dynamic array holding found primes */
static size_t cache_cap   = 0;     /* capacity of prime_cache */
static size_t cache_cnt   = 0;     /* number of primes stored */
static idx_t  next_k      = 1;     /* next k to process */

static const long double PI_VAL = 3.141592653589793238462643383279502884L;

/* Grow the prime cache to hold at least 'need' elements.  Returns true
   on success, false on allocation failure. */
static bool ensure_cache_capacity(size_t need) {
    if (need <= cache_cap) return true;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_cache) return false;          /* allocation failed */
    prime_cache = new_cache;
    cache_cap   = new_cap;
    return true;
}

/* Generate primes of the required form until the cache contains at least
   'n' primes.  If memory allocation fails, generation stops. */
static void generate_up_to(idx_t n) {
    while (cache_cnt < (size_t)n) {
        idx_t k = next_k++;
        /* triangular number T(k-1) = (k-1)*k/2 */
        idx_t tri = (k - 1) * k / 2;

        /* Compute floor(k + tri * π) using long double */
        long double val = (long double)tri * PI_VAL + (long double)k;
        uint64_t cand_val = (uint64_t)(val);  /* truncation gives floor */

        /* Test for primality */
        if (is_prime_big((seq_t)cand_val)) {
            if (!ensure_cache_capacity(cache_cnt + 1)) return; /* stop on allocation error */
            prime_cache[cache_cnt++] = (seq_t)cand_val;
        }
    }
}

/* Return the n-th term of the sequence (n >= 1).  Returns 0 for n == 0
   or if memory allocation fails before enough terms have been generated. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    generate_up_to(n);
    if (n <= cache_cnt) return prime_cache[n - 1];
    return 0; /* allocation failure or too few terms generated */
}
