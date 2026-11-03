/*
 * A176181  Primes p(n) such that gcd(p(n)-1, p(n+1)-1) > gcd(p(n)+1,
 *          p(n+1)+1).
 * https://oeis.org/A176181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 980 (2026-11-03)
 * Helpers:   gcd, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that
   gcd(p-1, next_prime(p)-1) > gcd(p+1, next_prime(p)+1) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Retrieve the n‑th value of the sequence.
   The function is memoised; any n ≥ 1 is supported. */
seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is offset‑1 */
        return (seq_t)0;

    /* Static cache of found primes and the last prime checked */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked_prime = 0;

    /* initialise on first call */
    if (cache_len == 0 && cache == NULL)
    {
        last_checked_prime = 2; /* start at the smallest prime */
    }

    /* extend the cache until we have at least n elements */
    while (cache_len < (size_t)n)
    {
        uint64_t p = last_checked_prime;          /* current prime */
        uint64_t q = next_prime(p);                /* next prime after p */

        /* compute the required gcds */
        seq_t g1 = gcd((seq_t)p - (seq_t)1, (seq_t)q - (seq_t)1);
        seq_t g2 = gcd((seq_t)p + (seq_t)1, (seq_t)q + (seq_t)1);

        if (g1 > g2)
        {
            /* store p in the cache */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }

        last_checked_prime = q;   /* continue from the next prime */
    }

    return cache[n - 1];
}
