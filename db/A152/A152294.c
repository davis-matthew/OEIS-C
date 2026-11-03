/*
 * A152294  Primes of the form : (p-n)/(n+1)=prime and (n+1)*p+n=prime. n=4.
 * https://oeis.org/A152294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
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

#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
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
    cache = mem_new(seq_t, cache_cap);
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
