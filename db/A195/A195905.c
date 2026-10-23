/*
 * A195905  Primes of the form 10 * k^2 + 7.
 * https://oeis.org/A195905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 3932, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime of the form 10*k^2+7, with k starting at 0
   (a(1)=7, a(2)=17, ...).  The function caches previously found primes
   so that calls in any order stay fast. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* static cache of found primes */
static seq_t *prime_cache = NULL;
static idx_t  cache_len  = 0;
static idx_t  cache_cap  = 0;
static idx_t  next_k     = 0;

/* make sure the cache can hold at least 'need' elements */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    while (cache_cap < need)
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
    /* ignore realloc failure for brevity */
}

/* choose the appropriate primality test for the current seq_t size */
static inline bool is_prime_seq(seq_t v)
{
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return is_prime_small((uint64_t)v);
    else
        return is_prime_big(v);
}

/* external function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of offset – undefined, return 0 */
        return (seq_t)0;

    /* find more primes until we have at least n of them */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = ((seq_t)10 * (k * k)) + (seq_t)7;

        if (is_prime_seq(val)) {
            ensure_cache_cap(cache_len + 1);
            prime_cache[cache_len] = val;
            ++cache_len;
        }
        ++next_k;
    }
    return prime_cache[n - 1];
}
