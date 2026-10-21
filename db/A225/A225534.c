/*
 * A225534  Numbers whose sum of cubed digits is prime.
 * https://oeis.org/A225534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth number (n ≥ 1) such that the sum of
   the cubes of its decimal digits is a prime.  The sequence starts
   11, 101, 110, 111, … ; see OEIS A225534.  The function is
   memoised so that arbitrary‐order calls with n beyond the
   previously computed range are efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_small.h"

/* Cache of already found numbers.  cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;        /* number of terms stored */
static idx_t  last_tested = 10;     /* integer immediately before the next one to test */

/* Grow the cache array to hold at least `min_cap` elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return true iff the sum of the cubes of the decimal digits of x is prime. */
static bool sum_of_cubes_is_prime(seq_t x)
{
    uint8_t buf[32];          /* enough for all practical seq_t values */
    size_t len;
    get_digits(x, buf, &len); /* provided by the framework */

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t d = (seq_t)buf[i];
        sum += d * d * d;
    }
    return is_prime_small(sum);   /* handles small integer sums */
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined – return 0 for safety */

    /* Ensure we have at least n terms cached. */
    if (cache_len < (size_t)n) {
        grow_cache(n);
        /* Scan integers greater than last_tested until we have n terms. */
        seq_t cand = (seq_t)(last_tested + 1);
        while (cache_len < (size_t)n) {
            if (sum_of_cubes_is_prime(cand)) {
                cache[cache_len++] = cand;
            }
            last_tested = (idx_t)cand;  /* last tested integer */
            ++cand;
        }
    }
    return cache[n - 1];
}
