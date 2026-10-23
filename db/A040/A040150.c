/*
 * A040150  Primes p such that x^4 = 21 has no solution mod p.
 * https://oeis.org/A040150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040150: primes p such that the congruence x^4 ≡ 21 (mod p) has no solution */
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Static state to cache computed terms and primes */
    static seq_t *cache = NULL;               /* array of computed sequence values */
    static idx_t cache_count = 0;             /* number of terms stored */
    static size_t cache_cap = 0;              /* allocated capacity of cache */

    static uint64_t *primes = NULL;           /* array of primes up to current limit */
    static size_t primes_cnt = 0;             /* current number of primes */
    static size_t primes_cap = 0;             /* current prime sieve limit */
    static size_t primes_index = 0;           /* next prime to test */

    /* Ensure we have at least n terms in the cache */
    while (cache_count < n) {
        /* If we have exhausted the current list of primes, extend the sieve */
        if (primes_index >= primes_cnt) {
            size_t new_limit = (primes_cap == 0) ? 1000 : primes_cap * 2;
            uint64_t *new_primes;
            size_t new_cnt;
            build_primes_upto(new_limit, &new_primes, &new_cnt);
            free(primes);                     /* free old array */
            primes = new_primes;
            primes_cnt = new_cnt;
            primes_cap = new_limit;
            /* primes_index remains at the same value, pointing past the processed primes */
        }

        /* Fetch the next prime to test */
        uint64_t p = primes[primes_index++];
        if (p <= 3) continue;                 /* 2,3 have solutions (x ≡ 0) */
        if (p == 7) continue;                 /* 7 divides 21, x ≡ 0 is a solution */

        seq_t mod = (seq_t)p;
        seq_t a_val = (seq_t)21 % mod;
        idx_t exp;
        if ((p - 1) % 4 == 0) {
            exp = (p - 1) / 4;
        } else {                               /* p ≡ 3 (mod 4) */
            exp = (p - 1) / 2;
        }

        /* If 21 is NOT a fourth power residue modulo p, add prime to cache */
        if (mod_pow(a_val, (uint64_t)exp, mod) != (seq_t)1) {
            ensure_capacity(cache_count + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_count++] = mod;
        }
    }

    return cache[n - 1];
}
