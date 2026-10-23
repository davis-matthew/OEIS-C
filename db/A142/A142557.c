/*
 * A142557  Primes congruent to 27 mod 53.
 * https://oeis.org/A142557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime ≡ 27 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 129, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* static cache of primes found so far */
static seq_t *primes_cache = NULL;        /* array of accumulated primes   */
static size_t cache_cap   = 0;            /* current capacity of the array */
static idx_t  cache_cnt   = 0;            /* number of primes stored       */
static seq_t  next_candidate = 27;        /* next value to test (≡27 mod 53) */

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(primes_cache, new_cap * sizeof(seq_t));
    primes_cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* ensure we have at least n primes stored */
    while (cache_cnt < n) {
        if (is_prime_big(next_candidate)) {
            grow_cache(cache_cnt + 1);
            primes_cache[cache_cnt++] = next_candidate;
        }
        next_candidate += (seq_t)53;
    }
    return primes_cache[n - 1];
}
