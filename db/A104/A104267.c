/*
 * A104267  Primes which are the concatenation of five consecutive square
 *          numbers.
 * https://oeis.org/A104267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digit_len, isprime_big, pow10_idx, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes which are the concatenation of five consecutive square numbers.
   This implementation iterates over consecutive integers m, concatenates the
   squares m²,(m+1)²,…,(m+4)², tests whether the concatenated number is prime,
   and stores the prime.  The function a(n) returns the n-th such prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/pow10_idx.h"
#include "../utils/isprime_big.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Cached primes and state for memoization */
static seq_t *prime_cache = NULL;   /* dynamic array of primes seen so far */
static idx_t cache_size = 0;        /* number of primes stored */
static idx_t cache_cap   = 0;        /* allocated capacity of prime_cache */
static idx_t m_current   = 0;        /* last m processed */

/* Return the n-th term (prime) of the sequence, n ≥ 1. */
seq_t a(idx_t n)
{
    /* Ensure cache array is allocated */
    if (prime_cache == NULL) {
        cache_cap   = 16;
        prime_cache = seq_malloc(cache_cap);
        cache_size  = 0;
    }

    /* Grow the cache array if needed */
    if (cache_size + 1 > cache_cap) {
        cache_cap <<= 1;
        prime_cache = seq_realloc(prime_cache, cache_cap);
    }

    /* Generate primes until we have at least n of them */
    while (cache_size < n) {
        m_current++;    /* next starting square index */

        seq_t concat = 0;
        for (idx_t k = 0; k < 5; ++k) {
            seq_t sq = (seq_t)(m_current + k) * (seq_t)(m_current + k);
            idx_t  d  = digit_len(sq);
            seq_t  mult = pow10_idx(d);
            concat = concat * mult + sq;
        }

        if (isprime_big(concat)) {
            /* store the prime */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                prime_cache = seq_realloc(prime_cache, cache_cap);
            }
            prime_cache[cache_size++] = concat;
        }
    }

    return prime_cache[n - 1];
}
