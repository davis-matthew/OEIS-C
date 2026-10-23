/*
 * A141920  Primes congruent to 16 mod 23.
 * https://oeis.org/A141920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 16 (mod 23)
 * Range:     u8 n <= 2, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/grow_seq.h"   /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */

static seq_t  *prime_cache = NULL;   /* cached primes ≡ 16 mod 23 */
static size_t cache_len   = 0;       /* number of cached primes */
static size_t cache_cap   = 0;       /* allocated capacity of the cache */
static idx_t  next_k      = 0;       /* next k in 16 + 23*k to test */

/* Return the n‑th prime congruent to 16 modulo 23.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* caller protection – undefined for n<1 */

    /* If we already have the answer, return it immediately. */
    if (n <= cache_len) return prime_cache[n - 1];

    /* Generate additional primes until we have at least n of them. */
    while (cache_len < n)
    {
        /* candidate = 16 + 23 * k  (k starts at 0) */
        idx_t candidate = 16 + 23 * next_k;
        ++next_k;

        /* Skip numbers less than 2 – the first candidates are 16 and 39, etc. */
        if (candidate < 2) continue;

        /* Test primality using the helper that works on idx_t. */
        if (is_prime(candidate))
        {
            /* Append the new prime to the cache. */
            grow_seq(&prime_cache, &cache_cap, cache_len + 1);
            prime_cache[cache_len++] = (seq_t)candidate;
        }
    }

    return prime_cache[n - 1];
}
