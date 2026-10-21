/*
 * A217614  Emirps p such that the next emirp is equal to the next prime.
 * https://oeis.org/A217614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Emirps p such that the next emirp equals the next prime.
   The list is generated on demand by searching successive primes
   and testing the emirp condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime.h"
#include "../utils/rev.h"

static seq_t *cache = NULL;      /* cached A217614 terms (seq_t type) */
static size_t cache_cap   = 0;    /* current capacity of cache */
static size_t cache_len   = 0;    /* number of terms stored */
static uint64_t last_prime = 2;   /* last prime that was examined */

/* return true if p is an emirp (prime, non‑palindromic, and its reverse is prime) */
static bool is_emirp(uint64_t p)
{
    if (p < 10)                     /* single‑digit primes are not emirps */
        return false;
    uint64_t revp = rev(p);
    if (revp == p)                  /* palindromic prime */
        return false;
    return is_prime(revp);
}

/* grow cache to hold room for another element and store val */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? 2 * cache_cap : 16;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* generate terms until we have at least `need` of them */
static void ensure_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        /* find next prime after last_prime */
        uint64_t p = next_prime(last_prime + 1);
        last_prime = p;

        if (is_emirp(p)) {
            uint64_t q = next_prime(p + 1); /* next prime after p */
            if (is_emirp(q))
                append_cache((seq_t)p);
        }
    }
}

/* external entry point required by the build system */
seq_t a(idx_t n)
{
    /* offset is 1; behaviour for n < 1 is irrelevant */
    if (n == 0)
        return 0;
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
