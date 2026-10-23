/*
 * A142549  Primes congruent to 19 mod 53.
 * https://oeis.org/A142549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 19 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 124, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"  /* assumes isPrimeGeneric(seq_t) returns bool */

/* Helper: ensure that a dynamic array has at least 'needed' slots */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* Cached list of primes ≡ 19 (mod 53) */
static seq_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_cap  = 0;
static idx_t next_k = 0;  /* k such that candidate = 19 + 53*k */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1 */
    if (n < 1) return 0;

    /* Fill cache until we have at least n primes */
    while (prime_cache_size < n) {
        seq_t candidate = (seq_t)19 + (seq_t)53 * (seq_t)next_k;
        if (isPrimeGeneric((seq_t)candidate)) {
            /* add to cache */
            ensure_capacity(prime_cache_size + 1, &prime_cache_cap,
                            (void **)&prime_cache, sizeof(seq_t));
            prime_cache[prime_cache_size++] = candidate;
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}
