/*
 * A200916  Primes of the form 7n^3-2.
 * https://oeis.org/A200916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form 7*n^3-2.
   a(n) is the n‑th such prime (OEIS A200916). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_capacity = 0;
static idx_t next_k = 1; /* the n used to generate 7*n^3-2 */

/* Append a new prime to the cache, expanding storage if necessary. */
static void add_prime(seq_t val)
{
    if (prime_cache_size == prime_cache_capacity) {
        size_t new_cap = prime_cache_capacity ? prime_cache_capacity * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        prime_cache = tmp;
        prime_cache_capacity = new_cap;
    }
    prime_cache[prime_cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1; undefined for n == 0 */

    while (prime_cache_size < n) {
        /* Compute 7*next_k^3 - 2 using 128‑bit intermediate to avoid overflow. */
        unsigned __int128 tmp = (unsigned __int128)next_k * next_k * next_k;
        tmp *= 7;
        tmp -= 2;
        seq_t val = (seq_t)tmp;

        if (is_prime_seq(val))
            add_prime(val);

        ++next_k;
    }
    return prime_cache[n - 1];
}
