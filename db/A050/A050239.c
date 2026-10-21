/*
 * A050239  Consecutive square palindromic primes; palindromic primes of the
 *          form x^2 + (x+1)^2, where x values are given by A050236.
 * https://oeis.org/A050239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard, base, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime of the form x^2 + (x+1)^2 that is also
   a palindrome (x is increased sequentially starting from 1). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached palindromic prime values   */
static size_t cache_cap = 0;         /* capacity of cache                 */
static size_t cache_len = 0;         /* number of terms currently cached  */
static idx_t   next_x   = 1;         /* next x to test (starts at 1)       */

/* Grow the cache so it can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1; /* double */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failed: abort */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n in the cache. */
    while (cache_len < (size_t)n) {
        seq_t x = (seq_t)next_x;
        seq_t val = x * x + (x + 1) * (x + 1);

        if (is_palindrome(val, 10) && is_prime_big(val)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_x;
    }
    return cache[n - 1];
}
