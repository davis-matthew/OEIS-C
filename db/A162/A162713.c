/*
 * A162713  Primes of the form 3^p-2^p-2 generated from primes p.
 * https://oeis.org/A162713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 3^p – 2^p – 2 where p is prime.
   The sequence a(n) lists these primes in the order of increasing p. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities available in the build */
#include "../utils/is_prime.h"      // bool is_prime(idx_t)
#include "../utils/is_prime_big.h"  // bool is_prime_big(seq_t)
#include "../utils/seq_mul.h"       // seq_t seq_mul(seq_t, seq_t)

/* ------------------------------------------------------------------ */
/* Main sequence generator                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static state persisted across calls */
    static bool initialized = false;
    static idx_t cur_exp = 0;            /* current exponent (p)            */
    static seq_t pow3 = (seq_t)1;         /* 3^cur_exp                       */
    static seq_t pow2 = (seq_t)1;         /* 2^cur_exp                       */
    static seq_t *cache = NULL;          /* cache of found primes           */
    static size_t cache_len = 0;         /* number of terms cached          */
    static size_t cache_cap = 0;         /* current cache capacity          */

    if (!initialized) {
        /* Nothing special to do on first call – state already zeroed. */
        initialized = true;
    }

    /* Generate further terms until we have at least n of them. */
    while (cache_len < n) {
        /* advance to next exponent */
        cur_exp++;
        pow3 = seq_mul(pow3, (seq_t)3);
        pow2 = seq_mul(pow2, (seq_t)2);

        /* If p is prime, test the candidate value */
        if (is_prime(cur_exp)) {
            seq_t val = pow3;
            val -= pow2;          /* val = 3^p - 2^p */
            val -= (seq_t)2;      /* val = 3^p - 2^p - 2 */
            if (is_prime_big(val)) {
                /* Ensure capacity in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failure */
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        }
    }

    return cache[n - 1];
}
