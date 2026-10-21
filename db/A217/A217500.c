/*
 * A217500  Primes of the form 2*n^2 + 74*n + 35.
 * https://oeis.org/A217500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 2518, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217500:
   a(k) is the k‑th prime of the form 2*n^2 + 74*n + 35.
   This implementation iteratively tests successive n starting at 1
   and collects the primes in a static cache. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *prime_cache = NULL;   /* array of found primes */
static size_t prime_cache_len = 0;   /* number of primes stored */
static size_t prime_cache_cap = 0;   /* allocated capacity */
static idx_t next_n = 1;             /* next n to test in the quadratic */

/* Ensure the cache has room for at least one more element. */
static void grow_cache(void)
{
    if (prime_cache_len == prime_cache_cap) {
        size_t new_cap = (prime_cache_cap == 0) ? 8 : prime_cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – terminate the program. */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
}

/* Return the k‑th prime of the quadratic form. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;               /* undefined for zero index */
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= prime_cache_len) {
        return prime_cache[(size_t)n - 1];
    }

    /* Extend the cache until we have the desired term. */
    while ((size_t)n > prime_cache_len) {
        seq_t nn = (seq_t)next_n;
        seq_t p  = 2 * nn * nn + 74 * nn + 35;  /* 2*n^2 + 74*n + 35 */
        if (isPrimeGeneric(p)) {
            grow_cache();
            prime_cache[prime_cache_len++] = p;
        }
        ++next_n;
    }

    return prime_cache[(size_t)n - 1];
}
