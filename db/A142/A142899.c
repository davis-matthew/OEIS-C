/*
 * A142899  Primes congruent to 19 mod 63.
 * https://oeis.org/A142899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: prime numbers congruent to 19 (mod 63).  Each call
   computes the nth element by extending a memoised list of such primes. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t); */

static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Append a new prime to the cache, resizing if necessary. */
static void push_prime(seq_t p)
{
    if (cache_size == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort as the sequence cannot continue. */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
    }
    prime_cache[cache_size++] = p;
}

/* Ensure that the cache contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache_size >= (size_t)need)
        return;                                   /* already have enough */

    idx_t candidate;
    if (cache_size == 0)
        candidate = 19;                            /* first term */
    else
        candidate = ((idx_t)prime_cache[cache_size - 1]) + 63;  /* next candidate */

    while ((idx_t)cache_size < need) {
        if (is_prime(candidate))
            push_prime((seq_t)candidate);
        candidate += 63;  /* next number ≡ 19 (mod 63) */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑indexed */
        return (seq_t)0;

    ensure_cache(n);
    return prime_cache[n - 1];
}
