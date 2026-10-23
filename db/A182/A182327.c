/*
 * A182327  Primes of the form 3^k + k + 1.
 * https://oeis.org/A182327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 3^k + k + 1 (OEIS A182327) */
/* The function a(n) returns the nth such prime, with a(1) the first term. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* Utilities provided by the build system */
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

/* Cached primes of the sequence */
static seq_t *prime_cache = NULL;   /* array of primes found so far */
static size_t cache_cap   = 0;      /* allocated capacity */
static size_t cache_len   = 0;      /* number of primes stored */

/* Current value of 3^k for the next candidate */
static idx_t   current_k  = 0;      /* k of the next candidate 3^k + k + 1 */
static seq_t   curr_pow   = (seq_t)1; /* 3^current_k */

/* Ensure the cache has enough capacity for needed elements. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                /* unrecoverable memory failure */
    prime_cache = new_cache;
    cache_cap   = new_cap;
}

/* Return the n‑th prime of the form 3^k + k + 1 (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 as a placeholder */

    /* Generate primes until we have at least n of them. */
    while (cache_len < n)
    {
        /* candidate = 3^k + k + 1 */
        seq_t candidate = curr_pow + (seq_t)current_k + (seq_t)1;

        if (is_prime_big(candidate))
        {
            ensure_capacity(cache_len + 1);
            prime_cache[cache_len++] = candidate;
        }

        /* Advance to the next k: 3^(k+1) = 3^k * 3 */
        curr_pow = curr_pow * (seq_t)3;
        ++current_k;
    }

    return prime_cache[n - 1];
}
