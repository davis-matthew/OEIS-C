/*
 * A077067  Squarefree numbers of the form prime + 1.
 * https://oeis.org/A077067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077067: squarefree numbers of the form prime + 1
 *
 * a(n) is the n-th integer that can be written as p+1 where p is prime
 * and the resulting number is square‑free.
 *
 * The function builds an incremental sieve of primes and tests each
 * candidate, storing every found value in a cache so that repeated
 * calls are answered in O(1).  The algorithm is O(n log n) and
 * comfortably handles n up to at least 10 000 within a fraction of a second.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree_uint64.h"

/* static space for generated terms */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* static space for primes */
static uint64_t *primes      = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static uint64_t last_checked = 1;   /* value last tested for primality */

/* ---------- helpers for dynamic arrays ---------- */
static void cache_grow(size_t needed)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();                     /* allocation failure */
    cache = new_buf;
    cache_cap = new_cap;
}

static void primes_grow(void)
{
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    uint64_t *new_buf = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_buf) abort();
    primes = new_buf;
    prime_cap = new_cap;
}

/* ---------- generate next prime (incremental) ---------- */
static uint64_t next_prime(void)
{
    for (;;)
    {
        ++last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > last_checked) break;
            if (last_checked % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            if (prime_cnt == prime_cap) primes_grow();
            primes[prime_cnt++] = last_checked;
            return last_checked;
        }
    }
}

/* ---------- main sequence accessor ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but harmless */

    /* grow cache until we have at least n terms */
    if (cache_len < n)
    {
        if (cache_len == 0) cache_grow(16);
        /* generate additional terms */
        while (cache_len < (size_t)n)
        {
            uint64_t p = next_prime();           /* next prime p */
            uint64_t val = p + 1;                  /* candidate p+1 */
            if (is_squarefree_uint64(val))
            {
                if (cache_len == cache_cap) cache_grow(cache_cap << 1);
                cache[cache_len++] = (seq_t)val;
            }
        }
    }
    return cache[n - 1];
}
