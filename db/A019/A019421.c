/*
 * A019421  Primes with primitive root 99.
 * https://oeis.org/A019421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, factor_u64, free_seq_array, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that 99 is a primitive root modulo p,
   i.e. the multiplicative order of 99 mod p equals p−1.
   The values are generated on demand by repeatedly selecting the
   next prime and testing it with factor_u64 and mod_pow. */

/* include standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility functions from the library */
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* helper: test whether 99 is a primitive root modulo p */
static bool is_primitive_root_99(uint64_t p)
{
    if (p == 2)                /* 99 ≡ 1 (mod 2) → order 1 = p−1 */
        return true;
    if (p % 3 == 0 || p % 11 == 0)
        return false;          /* 3 and 11 divide 99 → cannot be primitive */

    uint64_t n = p - 1;
    uint64_t primes[64], exps[64];
    size_t   cnt = 0;

    /* factorize n = p−1 */
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        uint64_t exp = n / q;
        /* if 99^((p−1)/q) ≡ 1 (mod p) then order divides (p−1)/q < p−1 */
        if (mod_pow((seq_t)99, exp, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for index 0 */
        return 0;

    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* array of found primes */
    static size_t cache_len  = 0;      /* number of primes stored */
    static size_t cache_cap  = 0;      /* capacity of cache array */
    static uint64_t next_start = 1;    /* next trial prime start */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = alloc_seq_array(cache_cap);
    }

    /* generate enough primes to satisfy the request */
    while (cache_len < (size_t)n) {
        /* get next candidate prime */
        uint64_t p = next_prime(next_start);
        next_start = p + 1;

        /* test if p has 99 as a primitive root */
        if (is_primitive_root_99(p)) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                if (new_cap == 0) new_cap = 64;
                seq_t *tmp = alloc_seq_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                free_seq_array(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
