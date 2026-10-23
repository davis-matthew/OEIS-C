/*
 * A191030  Primes that are quadratic residues mod 41.
 * https://oeis.org/A191030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that 41 is a quadratic residue
 *            modulo p (legendre symbol (41|p) = 1). The prime sequence is
 *            generated lazily and cached so that arbitrary indexed calls work
 *            efficiently
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/powmod64.h"

/* static cache for already found terms */
static seq_t *cache = NULL;          /* array of found primes */
static idx_t cache_len = 0;          /* number of elements in cache */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static uint64_t last_checked = 1;    /* last prime examined */

/* grow the cache twice as large */
static void cache_grow(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(newp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Legendre symbol test: returns 1 if 41 is a quadratic residue mod p,
   0 otherwise.  p must be an odd prime. */
static int is_residue41(uint64_t p)
{
    /* when p == 41 the symbol is 0, so this will return 0. */
    uint64_t res = powmod64(41ull, (p - 1) / 2, p);
    return res == 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset, but harmless */

    /* generate terms until we have at least n terms */
    while (cache_len < n) {
        /* find the next prime */
        last_checked = (last_checked < 2) ? 2 : next_prime(last_checked + 1);

        /* test Legendre symbol (41|p) */
        if (is_residue41(last_checked)) {
            if (cache_len >= cache_cap) cache_grow();
            cache[cache_len++] = (seq_t)last_checked;
        }
        /* otherwise skip this prime and continue */
    }

    return cache[n - 1];
}
