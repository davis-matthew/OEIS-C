/*
 * A038986  Primes p such that 95 is not a square mod p.
 * https://oeis.org/A038986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p for which 95 is not a quadratic residue
 *            mod p (i.e. (95/p) = -1)
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

/* static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_sz = 0;   /* number of values stored */
static size_t cache_cap = 0;  /* array capacity */
static uint64_t next_candidate = 3; /* next odd number to test */

/* ensure the cache has room for at least 'need' elements */
static inline void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Legendre symbol (a|p) for odd prime p:
   returns 1 if a is a quadratic residue mod p,
   returns p-1 if a is a nonresidue (i.e. -1 mod p).
   This uses Euler’s criterion with powmod64. */
static inline uint64_t legendre_95(uint64_t p)
{
    /* p is prime and odd */
    return powmod64(95, (p - 1) >> 1, p);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safer */
    while (cache_sz < n)
    {
        /* skip non‑primes */
        while (!is_prime(next_candidate))
            next_candidate += 2;

        /* 95 is a quadratic non‑residue iff legendre_95(p) == p-1 */
        if (legendre_95(next_candidate) == next_candidate - 1)
        {
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = (seq_t)next_candidate;
        }

        next_candidate += 2; /* proceed to next odd candidate */
    }
    return cache[n - 1];
}
