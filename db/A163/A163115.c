/*
 * A163115  Primes of the form 2^n + 2*n + 1.
 * https://oeis.org/A163115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163115:
   a(n) = the nth prime of the form 2^k + 2*k + 1.
   The function memoises the results so that it can answer
   queries in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp);          */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n);     */

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache   = NULL;   /* array of discovered primes */
    static size_t cache_sz = 0;     /* number of primes stored     */
    static size_t cache_cap = 0;    /* capacity of cache           */

    /* next exponent k to test */
    static idx_t next_k = 0;

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate primes until we have at least n of them */
    while (cache_sz < n) {
        /* candidate value = 2^k + 2*k + 1 */
        seq_t val = pow2(next_k);
        val += (seq_t)next_k * (seq_t)2;
        val += (seq_t)1;

        /* test for primality; if prime, store it */
        if (is_prime_big(val)) {
            if (cache_sz == cache_cap) {
                cache_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
