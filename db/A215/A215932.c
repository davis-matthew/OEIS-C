/*
 * A215932  Happy reversible primes.
 * https://oeis.org/A215932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   get_digits, is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of those primes that are happy and whose
 * decimal reversal is also a happy prime.  To generate a(n) we
 * enumerate primes in increasing order, test happiness for the
 * prime and for its reverse, and store the qualifying primes in
 * a cache.  The function is memoised, so any permutation of
 * calls to a(n) gives correct results quickly.  The algorithm
 * requires only primality testing and digit extraction – both
 * provided by the utility functions. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/rev_decimal.h"   /* seq_t rev_decimal(seq_t) */
#include "../utils/get_digits.h"    /* void get_digits(seq_t, uint8_t *, size_t *) */

static inline bool is_happy(seq_t x)
{
    /* Determine happiness by iterating sums of digit squares. */
    while (x != ((seq_t)1) && x != ((seq_t)4)) {
        uint64_t sum = 0;
        uint8_t buf[32];
        size_t len;
        get_digits(x, buf, &len);
        for (size_t i = 0; i < len; ++i) {
            uint64_t d = buf[i];
            sum += d * d;
        }
        x = (seq_t)sum;
    }
    return x == ((seq_t)1);
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    /* Current candidate to test */
    static idx_t   candidate  = 2;
    static bool    started    = false;

    /* Initialise cache on first call */
    if (!started) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        started   = true;
    }

    /* Fill cache until we have at least n elements */
    while (cache_len < n) {
        /* Test primality */
        if (!is_prime_big((seq_t)candidate)) {
            candidate++;
            continue;
        }

        /* Test happiness of the prime itself */
        if (is_happy((seq_t)candidate)) {
            /* Reverse the number */
            seq_t rev = rev_decimal((seq_t)candidate);
            /* Test primality and happiness of the reverse */
            if (is_prime_big(rev) && is_happy(rev)) {
                /* Ensure cache capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_arr) { /* allocation failed; abort gracefully */
                        free(cache);
                        cache = NULL;
                        return (seq_t)0;
                    }
                    cache     = new_arr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        candidate++;
    }

    return cache[n - 1];
}
