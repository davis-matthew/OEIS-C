/*
 * A140842  Primes of the form 210k + 17.
 * https://oeis.org/A140842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 210k + 17 (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* A small dynamic cache of previously found primes of the required form */
static seq_t *prime_cache    = NULL;  /* array of primes */
static size_t prime_cache_size = 0;   /* number of stored primes */
static size_t prime_cache_cap  = 0;   /* current allocated capacity */
static idx_t   next_k           = 0;  /* next value of k to test */

/* Append a new prime to the cache, expanding storage if necessary */
static void cache_push(seq_t value)
{
    if (prime_cache_size == prime_cache_cap) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate as the sequence cannot be continued */
            exit(EXIT_FAILURE);
        }
        prime_cache   = tmp;
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_size++] = value;
}

/* Return the n-th prime of the form 210k + 17, 1‑based indexing */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is undefined for n=0; return zero to signal invalid index */
        return 0;
    }

    /* If the requested prime is already cached, return it immediately */
    if ((size_t)(n - 1) < prime_cache_size) {
        return prime_cache[n - 1];
    }

    /* Generate further candidates until the cache holds at least n primes */
    while (prime_cache_size < n) {
        seq_t candidate = 210 * (seq_t)next_k + 17;
        if (is_prime_big(candidate)) {
            cache_push(candidate);
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}
