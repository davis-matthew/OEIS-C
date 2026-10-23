/*
 * A152294  Primes of the form : (p-n)/(n+1)=prime and (n+1)*p+n=prime. n=4.
 * https://oeis.org/A152294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 25, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152294: primes p such that (p-4)/5 and 5p+4 are prime. */

/* The function walks over primes q, forms p = 5*q+4, and checks
   primality of p and 5*p+4.  Results are cached so that out‑of‑order
   calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t, uint64_t **, size_t *); */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n); */

static seq_t *cache = NULL;     /* cached sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t *prime_list = NULL;   /* list of primes q */
static size_t prime_cnt  = 0;         /* number of primes in list */
static size_t prime_next = 0;         /* next prime index to use */

static idx_t prime_limit = 0;
static bool initialized = false;

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

static void rebuild_prime_list(idx_t new_limit)
{
    if (prime_list) free(prime_list);
    build_primes_upto(new_limit, &prime_list, &prime_cnt);
    prime_limit = new_limit;
    /* prime_next remains unchanged: primes are prefix‑stable */
}

static void init_state(void)
{
    if (initialized) return;
    initialized = true;

    cache_cap = 1024;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) abort();

    /* Initial limit should cover many terms – start with a modest
       value and grow as needed. */
    prime_limit = 1000000ULL;
    rebuild_prime_list(prime_limit);
}

static void generate_terms(idx_t target)
{
    for (; cache_len < target; ) {
        if (prime_next == prime_cnt) {
            /* Not enough prime q's – enlarge the prime list. */
            idx_t new_limit = prime_limit * 2;
            rebuild_prime_list(new_limit);
            continue;
        }

        uint64_t q = prime_list[prime_next++];
        seq_t p = (seq_t)5 * (seq_t)q + (seq_t)4;
        if (!is_prime_big(p)) continue;          /* p must be prime */

        seq_t r = (seq_t)5 * p + (seq_t)4;       /* 5p+4 */
        if (!is_prime_big(r)) continue;          /* that must be prime too */

        grow_cache(cache_len + 1);
        cache[cache_len++] = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe return */

    init_state();
    generate_terms(n);
    return cache[n-1];
}
