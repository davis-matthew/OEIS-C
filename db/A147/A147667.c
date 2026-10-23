/*
 * A147667  Primes of the form 5^k - 4^k.
 * https://oeis.org/A147667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of the form 5^k - 4^k, where k is a positive
 *            integer (odd k because even k produce a multiple of 3)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Helpers:   big_pow, is_prime_big, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

/* static cache of found primes and state of the search */
static seq_t *cache = NULL;          /* array of primes found */
static idx_t cache_len = 0;          /* number of primes stored */
static size_t cache_cap = 0;         /* current capacity of cache */
static bool cache_ready = false;     /* whether initialization has been performed */

/* current exponent and powers of 5 and 4 that correspond to the last k examined */
static idx_t   k_curr   = 0;   /* current k (always odd after first init) */
static seq_t   pow5_curr = 0;  /* 5^k_curr   */
static seq_t   pow4_curr = 0;  /* 4^k_curr   */

/* extend the cache until it contains at least n primes */
static void extend_to(idx_t n)
{
    /* first time: initialise to the first odd k = 3 */
    if (!cache_ready) {
        cache_ready = true;
        k_curr   = 3;
        pow5_curr = big_pow(5, (seq_t)k_curr);
        pow4_curr = big_pow(4, (seq_t)k_curr);
    }

    while (cache_len < n) {
        /* value for the current k */
        seq_t val = pow5_curr - pow4_curr;

        if (is_prime_big(val)) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        /* advance to the next odd exponent k + 2 */
        k_curr += 2;
        pow5_curr = seq_mul(pow5_curr, 25);   /* 5^(k+2) = 5^k * 5^2 */
        pow4_curr = seq_mul(pow4_curr, 16);   /* 4^(k+2) = 4^k * 4^2 */
    }
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, but guard against zero */
    extend_to(n);
    return cache[n - 1];
}
