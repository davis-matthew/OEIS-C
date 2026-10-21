/*
 * A215106  Primes congruent to {3, 5, 6} mod 11.
 * https://oeis.org/A215106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table: primes congruent to 3, 5 or 6 modulo 11
   Computation method: incremental search using is_prime_big.
   The first terms are 3, 5, 17, 47, …
*/

#include <stdbool.h>
#include <stdlib.h>
/* retrieves the function bool is_prime_big(seq_t); */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state: cache of found primes and next candidate to test */
    static seq_t *cache   = NULL;   /* array of primes found so far */
    static size_t cache_cap = 0;    /* capacity of cache */
    static size_t cache_size = 0;   /* number of primes stored */
    static seq_t next_candidate = (seq_t)3;  /* next odd number to test */
    static bool initialized = false;        /* flag for one‑time init */

    /* initialise only once */
    if (!initialized) {
        /* first prime in the sequence is 3 */
        initialized = true;
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        seq_t mod = next_candidate % (seq_t)11;   /* remainder modulo 11 */

        if (mod == (seq_t)3 || mod == (seq_t)5 || mod == (seq_t)6) {
            if (is_prime_big(next_candidate)) {
                /* enlarge cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();                /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = next_candidate;
            }
        }

        next_candidate += (seq_t)2;   /* skip even numbers */
    }

    return cache[n - 1];
}
