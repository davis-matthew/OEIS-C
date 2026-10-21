/*
 * A057737  Primes of the form 2^p + 3, where p is prime.
 * https://oeis.org/A057737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p + 3, where p is the nth prime
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

/*--- Cached terms -------------------------------------------------------*/
static seq_t *cache = NULL;      /* array holding found terms           */
static size_t cache_size = 0;    /* number of terms currently stored    */
static size_t cache_cap  = 0;    /* current allocation size             */

/*--- Progress marker ----------------------------------------------------*/
/* The last prime examined by the generator. */
static uint64_t last_checked_prime = 1;

/*--- Exponent limit for fixed‑width seq_t ------------------------------*/
static bool limit_inited = false;
static unsigned max_exponent_limit = 0;

/*--- Ensure cache capacity ---------------------------------------------*/
static inline void ensure_cache_capacity(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Initialise exponent limit on first call. */
    if (!limit_inited) {
        limit_inited = true;
        /* For fixed‑width unsigned types the limit is (bits - 1). */
        if (sizeof(seq_t) <= 8) {
            /* Unsigned 64‑bit: 2^63+3 < UINT64_MAX → max exp = 63. */
            max_exponent_limit = (sizeof(seq_t) * 8) - 1;
        } else {
            /* Larger unsigned types (e.g. __int128) or arbitrary precision
               objects – no practical limit needed. Use the largest
               possible exponent. */
            max_exponent_limit = (unsigned)-1;
        }
    }

    /* Extend the cached sequence until it contains at least n terms. */
    while (cache_size < n) {
        /* Get the next prime p > last_checked_prime. */
        uint64_t p = next_prime(last_checked_prime);
        last_checked_prime = p;

        /* If seq_t is a fixed‑width type, stop when the exponent would
           overflow. */
        if (max_exponent_limit != (unsigned)-1 && p > max_exponent_limit)
            break;

        /* Compute 2^p + 3. */
        seq_t val = pow_uint(2, p);
        val += (seq_t)3;

        /* Keep the term only if it is prime. */
        if (is_prime_big(val)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
            if (cache_size >= n)
                return cache[n - 1];
        }
    }

    /* If we did not gather enough terms, the requested index is out of
       range for this build. Return 0. */
    return (seq_t)0;
}
