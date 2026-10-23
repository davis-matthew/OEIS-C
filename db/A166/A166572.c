/*
 * A166572  Prime numbers containing the string 11.
 * https://oeis.org/A166572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime containing the decimal substring "11"
 * Range:     u8 n <= 3, u16 n <= 323, u32 n <= 324, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* primality test for arbitrary-precision seq_t values */
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* array of found primes          */
static size_t cache_cap  = 0;       /* current capacity of cache     */
static size_t cache_cnt  = 0;       /* number of primes stored       */
static idx_t   next_candidate = 11; /* next integer to test (idx_t)  */

/* enlarge the cache array if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < needed) {
        cache_cap <<= 1;                           /* double it               */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* test whether a decimal representation of n contains “11” */
static bool contains_11(seq_t n)
{
    seq_t prev = (seq_t)0;   /* digit sentinel, 0 cannot be *both* 1 at once */
    while (n > (seq_t)0) {
        seq_t digit = n % (seq_t)10;
        if (digit == (seq_t)1 && prev == (seq_t)1)
            return true;
        prev   = digit;
        n     /= (seq_t)10;
    }
    return false;
}

/* find the next prime ≥ next_candidate that contains “11” */
static seq_t find_next_prime_with_11(void)
{
    for (;;) {
        if (next_candidate < 2)
            next_candidate = 2;
        seq_t cand = (seq_t)next_candidate;
        if (contains_11(cand) && is_prime_big(cand)) {
            next_candidate++;      /* next search starts at cand+1 */
            return cand;
        }
        ++next_candidate;
    }
}

/* required public sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;           /* undefined for n<1, but harmless */

    /* guarantee we have at least n primes in the cache */
    if (cache_cnt < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        while (cache_cnt < (size_t)n) {
            seq_t p = find_next_prime_with_11();
            cache[cache_cnt++] = p;
        }
    }
    return cache[n - 1];
}
