/*
 * A142759  Primes congruent to 32 mod 59.
 * https://oeis.org/A142759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 32 (mod 59) (n ≥ 1)
 * Range:     u8 none, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"    /* declares bool is_prime(idx_t) */

/* Cache for already discovered primes ≡ 32 (mod 59) */
static seq_t *prime_cache = NULL;   /* dynamic array of seq_t */
static size_t cache_len  = 0;        /* number of primes stored          */
static size_t cache_cap  = 0;        /* current capacity                  */
static idx_t next_candidate = 32;    /* next integer ≡ 32 (mod 59) to test */

/* ------------------------------------------------------------------ */
/* Resize the cache array so that it can hold at least `needed` elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    /* Reallocate the array of seq_t; the cast satisfies the compiler. */
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        /* Allocation failed – terminate the program safely. */
        /* The OEIS harness should never hit this for the required limits. */
        abort();
    }
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Append a newly found prime to the cache. */
static void add_prime_to_cache(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    prime_cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Return the n-th prime congruent to 32 (mod 59).                         */
seq_t a(idx_t n)
{
    if (n == 0)          /* guard against out‑of‑bounds callers */
        return 0;

    /* Discover missing primes until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            add_prime_to_cache((seq_t)next_candidate);
        }
        next_candidate += 59;   /* advance to the next candidate */
    }

    return prime_cache[n - 1];
}
