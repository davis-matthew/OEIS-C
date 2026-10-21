/*
 * A053177  Odd composite k such that (k-1)/2 is prime.
 * https://oeis.org/A053177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2949, u32 n <= 3263, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates terms of OEIS sequence A053177:
   odd composite k such that (k-1)/2 is prime.  */
/* The sequence is obtained by iterating over primes p and
   considering k = 2p+1.  If k is composite, it is added to the
   cache.  The cache is grown lazily to answer any random
   request a(idx_t n).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* ---------- prime list ---------- */
static uint64_t *prime_list = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;
static size_t   prime_index = 0;          /* next prime to use */
static uint64_t prime_last_checked = 1;   /* last number checked (inclusive) */

/* ensure prime list contains all primes up to limit (inclusive) */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_last_checked)
        return;          /* nothing new to check */

    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_len = 1;
    }

    for (uint64_t cand = prime_last_checked + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        if ((cand & 1u) == 0) {
            if (cand == 2) continue;
            continue;    /* skip even numbers > 2 */
        }
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;
        }
    }
    prime_last_checked = limit;
}

/* ---------- cache of sequence terms ---------- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

static void add_cache_term(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for 0‑th index; return 0 safely */
        return (seq_t)0;

    /* Generate enough terms to satisfy the request */
    while (cache_len < (size_t)n) {
        /* Ensure we still have primes to consume */
        if (prime_index >= prime_len) {
            /* Extend the prime list by a factor of 2 each time */
            uint64_t new_limit = prime_last_checked * 2 + 1;
            ensure_primes_upto(new_limit);
            continue;
        }

        uint64_t p = prime_list[prime_index++];
        seq_t  k = (seq_t)(2 * p + 1);    /* k = 2p + 1 */

        if (!is_prime_big(k))            /* check composite */
            add_cache_term(k);
    }

    return cache[(size_t)n - 1];
}
