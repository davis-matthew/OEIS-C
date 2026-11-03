/*
 * A019387  Primes with primitive root 63.
 * https://oeis.org/A019387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p for which 63 is a primitive root modulo p
 *            (OEIS A019387). The definition is computed by searching primes
 *            and testing the primitive root property
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 966 (2026-11-01)
 * Helpers:   factorize, is_prime, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factorize.h"
#include "../utils/is_prime.h"
#include "../utils/powmod_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static cache of the sequence values */
static seq_t *cache = NULL;        /* array of found primes */
static idx_t cache_len = 0;        /* number of terms currently cached */
static size_t cache_cap = 0;       /* allocated capacity of cache in elements */
static uint64_t last_checked = 1;  /* last integer tested for being a term */

/* enlarge the cache using the library's reallocator */
static void _grow_cache(size_t new_cap)
{
    seq_t *newp = seq_realloc(cache, cache_cap, new_cap);
    cache = newp;
    cache_cap = new_cap;
}

/* test whether p is a prime for which 63 is a primitive root modulo p */
static bool _is_primitive_root_63(uint64_t p)
{
    if (p == 2)
        return true;                  /* 63 ≡ 1 (mod 2), order 1 = p‑1 */

    /* 63 must be coprime to p; exclude any prime divisor of 63 */
    if ((63ULL % p) == 0)
        return false;                 /* p | 63 ⇒ 63 ≡ 0 (mod p), not a unit */

    uint64_t m = p - 1;

    /* get prime factors of m; m < 2^64 so 64 entries are enough */
    uint64_t prime_factors[64];
    int exps[64];
    size_t cnt = factorize(m, prime_factors, exps);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = prime_factors[i];
        /* order of 63 modulo p must not divide m/q -> test 63^(m/q) ≡ 1? */
        if (powmod_u64(63ULL, m / q, p) == 1ULL)
            return false;              /* not a primitive root */
    }
    return true;                       /* all tests passed */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; this case should not arise */

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* find next candidate integer */
        last_checked++;

        /* handle integer 2 specially */
        if (last_checked == 2) {
            if (cache_len == cache_cap)
                _grow_cache(cache_cap == 0 ? 4 : cache_cap * 2);
            cache[cache_len++] = (seq_t)2;
            continue;
        }

        /* skip even numbers except 2 */
        if ((last_checked & 1ULL) == 0)
            continue;

        /* check primality */
        if (!is_prime(last_checked))
            continue;

        /* test primitive root property */
        if (_is_primitive_root_63(last_checked)) {
            if (cache_len == cache_cap)
                _grow_cache(cache_cap == 0 ? 4 : cache_cap * 2);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* return the nth term (1‑indexed) */
    return cache[n - 1];
}
