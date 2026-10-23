/*
 * A040101  Primes p such that x^4 = 3 has a solution mod p.
 * https://oeis.org/A040101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that x^4 ≡ 3 (mod p)
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Primes are: 2, 3, all primes congruent to 11 (mod 12), and primes p ≡ 1 (mod 4) with 3^((p-1)/4) ≡ 1 (mod p). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Utility functions from the framework */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n);        */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/seq_alloc_new_array.h" /* seq_t* seq_alloc_new_array(size_t n); */
#include "../utils/seq_free_array.h"      /* void seq_free_array(seq_t *p); */

static seq_t *cache = NULL;          /* array of found primes of this sequence */
static idx_t cache_size = 0;         /* number of stored elements */
static idx_t cache_cap   = 0;         /* capacity of the array   */
static idx_t next_candidate = 2;      /* next integer to test for primality */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (cache != NULL) {
        for (size_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        seq_free_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n < 1, but harmless */
    /* Ensure the cache holds at least n terms */
    while (cache_size < n) {
        /* Find the next prime starting from next_candidate */
        if (!is_prime(next_candidate))
            next_candidate++;
        else {
            /* Candidate is prime */
            seq_t val = (seq_t)next_candidate;
            bool accept =
                (next_candidate == 2) ||
                (next_candidate == 3) ||
                (next_candidate % 12 == 11) ||  /* 11 (mod 12) */
                ( (next_candidate % 4 == 1) &&
                  (mod_pow((seq_t)3, (next_candidate - 1) / 4, val) == (seq_t)1) );
            if (accept) {
                if (cache_size == cache_cap)
                    grow_cache();
                cache[cache_size++] = val;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
