/*
 * A218582  Primes which do not divide any Mersenne number M(p) = 2^p - 1 with
 *          prime p.
 * https://oeis.org/A218582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 6130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A218582: primes that do not divide any Mersenne number M(p) = 2^p – 1 (p prime) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined, but return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Static state */
    static uint64_t *all_primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;
    static size_t idx_next = 0;          /* next prime to test */

    static seq_t *valid = NULL;
    static size_t valid_cnt = 0;
    static size_t valid_cap = 0;

    /* initial limit for primes (adjustable if needed) */
    static const idx_t INIT_LIMIT = 200000;

    /* Flag to build primes only once */
    static bool initialized = false;

    if (!initialized) {
        primes_limit = INIT_LIMIT;
        build_primes_upto(primes_limit, &all_primes, &primes_cnt);
        idx_next = 0;
        valid_cap = 64;
        valid = (seq_t *)malloc(valid_cap * sizeof(seq_t));
        valid_cnt = 0;
        initialized = true;
    }

    /* Expand primes and recompute as necessary to gather enough terms */
    while (valid_cnt < (size_t)n) {
        /* Ensure we have enough candidate primes */
        if (idx_next >= primes_cnt) {
            /* Need more primes: double the limit */
            primes_limit <<= 1;  /* multiply by 2 */
            /* free old array? not needed for correctness, but free to avoid leaks */
            free(all_primes);
            build_primes_upto(primes_limit, &all_primes, &primes_cnt);
            idx_next = 0;  /* restart from beginning if we want to re-evaluate old primes,
                              but we can continue from idx_next since new primes are appended
                              after old ones. */
        }

        uint64_t q = all_primes[idx_next++];
        /* Check if q divides any M(p) */
        bool excluded = false;
        uint64_t temp = q - 1;
        for (size_t i = 0; i < primes_cnt && all_primes[i] * all_primes[i] <= temp; ++i) {
            uint64_t p = all_primes[i];
            if (temp % p == 0) {
                seq_t r = mod_pow((seq_t)2, (idx_t)p, (seq_t)q);
                if (r == (seq_t)1) {
                    excluded = true;
                    break;
                }
                while (temp % p == 0) {
                    temp /= p;
                }
            }
        }
        if (!excluded && temp > 1) {  /* remaining prime factor */
            seq_t r = mod_pow((seq_t)2, (idx_t)temp, (seq_t)q);
            if (r == (seq_t)1) {
                excluded = true;
            }
        }

        if (!excluded) {
            if (valid_cnt >= valid_cap) {
                valid_cap = valid_cap ? valid_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(valid, valid_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Memory allocation failed, abort */
                    return (seq_t)0;
                }
                valid = tmp;
            }
            valid[valid_cnt++] = (seq_t)q;
        }
    }

    return valid[n - 1];
}
