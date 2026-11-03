/*
 * A145484  Primes p such that 2*p - 29 is a positive prime.
 * https://oeis.org/A145484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9447 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 2*p-29 is a positive prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"
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

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;       /* cached a(k) values */
    static size_t cache_size = 0;     /* number of cached terms */
    static size_t cache_cap  = 0;     /* allocated capacity */

    static idx_t *prime_list = NULL;  /* list of primes */
    static size_t prime_cnt  = 0;     /* number of primes in the list */
    static idx_t prime_idx  = 0;      /* next prime to examine */

    static bool initialised = false;

    /* Helper: grow cache if needed */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise once: generate a first prime list */
    if (!initialised) {
        idx_t limit = 500000;   /* first sieve limit */
        build_primes(limit, &prime_list, &prime_cnt);
        initialised = true;
    }

    /* expand prime list until we have enough terms */
    while (cache_size < (size_t)n) {
        /* If we've exhausted current primes, enlarge the sieve */
        if (prime_idx >= prime_cnt) {
            idx_t old_limit  = prime_list[prime_cnt - 1] + 1;
            idx_t new_limit  = old_limit * 2;
            free(prime_list);
            build_primes(new_limit, &prime_list, &prime_cnt);
            /* prime_idx already points to the next new prime */
        }

        /* take next prime p from the list */
        seq_t p = (seq_t)prime_list[prime_idx++];
        /* compute q = 2*p - 29 */
        seq_t q = (seq_t)2 * p - (seq_t)29;

        /* test if q is a positive prime */
        if (q > (seq_t)0 && is_prime_big(q)) {
            /* store p into cache */
            cache[cache_size++] = p;
        }

        /* ensure cache capacity for next insertion if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    return cache[n - 1];
}
