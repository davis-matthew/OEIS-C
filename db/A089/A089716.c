/*
 * A089716  Primes that are both congruent to 1 mod 10 and congruent to 1 or 2
 *          mod 9.
 * https://oeis.org/A089716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p % 10 == 1 and (p % 9 == 1 or p % 9 == 2)
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cache of already found terms (static to preserve across calls) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow the cache so that it can hold at least n elements */
static void ensure_cache(size_t n)
{
    if (cache_len >= n)
        return;

    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Start searching from the number that follows the last cached prime */
    seq_t cur = (cache_len == 0) ? (seq_t)11 : cache[cache_len - 1] + (seq_t)10;

    while (cache_len < n) {
        /* cur is guaranteed to be ≡1 (mod 10) by construction */
        if (is_prime_big(cur)) {
            int r9 = (int)(cur % (seq_t)9);
            if (r9 == 1 || r9 == 2)
                cache[cache_len++] = cur;
        }
        cur += (seq_t)10;          /* next candidate still ≡1 (mod 10) */
    }
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    /* The sequence has offset 1 (a(1) is the first term). */
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, but harmless */

    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
