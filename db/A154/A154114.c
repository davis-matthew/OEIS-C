/*
 * A154114  Primes p such that p + 9699690 is also prime, where 9699690 is the
 *          8th primorial number A002110(8).
 * https://oeis.org/A154114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9481 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th prime p such that p + 9699690 is also prime.
 * (9699690 = 2·3·5·7·11·13·17·19, the 8th primorial). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* ------------------------------------------------------------------- */
/*                                                                    */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0.  Returning 0 prevents UB. */
    if (n == 0)
        return (seq_t)0;

    /* ---------------------------------------------------------------- */
    /* Static cache for found terms.                                   */
    static seq_t *cache     = NULL;   /* array of seq_t values          */
    static size_t cache_len = 0;     /* current number of terms stored */
    static size_t cache_cap = 0;     /* current capacity (in elements) */

    /* ---------------------------------------------------------------- */
    /* Static sieve data.                                               */
    static uint64_t *primes            = NULL;   /* prime table              */
    static size_t    primes_cnt       = 0;      /* number of stored primes  */
    static idx_t     last_prime_index = 0;      /* next prime to examine    */
    static idx_t     sieve_limit     = 0;      /* current sieve size       */

    /* Initialise cache. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return (seq_t)0;     /* safety */
    }

    /* Initialise sieve (first call). */
    if (primes == NULL) {
        sieve_limit = 2000000;                  /* start with 2 M */
        build_primes_upto(sieve_limit, &primes, &primes_cnt);
    }

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* If we ran out of primes in the current sieve, double it. */
        if (last_prime_index >= primes_cnt) {
            idx_t new_limit = sieve_limit * 2;
            uint64_t *new_primes = NULL;
            size_t new_cnt = 0;
            build_primes_upto(new_limit, &new_primes, &new_cnt);
            free(primes);
            primes    = new_primes;
            primes_cnt = new_cnt;
            sieve_limit = new_limit;
            /* last_prime_index already points just past the old array,
               which is now valid after reallocation. */
        }

        /* Scan remaining primes in the current sieve. */
        while (cache_len < (size_t)n && last_prime_index < primes_cnt) {
            uint64_t p = primes[last_prime_index++];
            /* Check primality of p and p+9699690. */
            if (is_prime_big((seq_t)p) &&
                is_prime_big((seq_t)(p + 9699690))) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (tmp == NULL) return (seq_t)0; /* safety */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
    }

    return cache[n - 1];
}
