/*
 * A142228  Primes congruent to 31 mod 41.
 * https://oeis.org/A142228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime ≡ 31 (mod 41) (OEIS A142228) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* cache of computed primes */
static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t next_candidate = 31;   /* next number to test */

/* ensure cache can hold at least `needed` primes */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed + 16;          /* grow factor */
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                           /* allocation failed */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset 1, undefined for n = 0 */
        return 0;

    if ((idx_t)cache_size >= n)
        return prime_cache[n - 1];

    /* continue generating primes until we have at least n of them */
    while ((idx_t)cache_size < n) {
        if (is_prime_small((seq_t)next_candidate)) {
            ensure_cache(cache_size + 1);
            prime_cache[cache_size] = (seq_t)next_candidate;
            ++cache_size;
        }
        next_candidate += 41;              /* next number ≡ 31 (mod 41) */
    }

    return prime_cache[n - 1];
}
