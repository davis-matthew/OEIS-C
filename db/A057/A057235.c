/*
 * A057235  Numbers n such that n | 6^n + 5^n + 4^n.
 * https://oeis.org/A057235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth natural number k such that k divides 6^k + 5^k +
 *            4^k
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a simple incremental search with memoisation. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* found terms, in order */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated space */
static idx_t next_candidate = 1;     /* number to test next (>=1) */

/* test whether x | 6^x + 5^x + 4^x   (modular test) */
static bool good(idx_t x)
{
    seq_t mod = (seq_t)x;
    seq_t r6 = mod_pow(6, x, mod);
    seq_t r5 = mod_pow(5, x, mod);
    seq_t r4 = mod_pow(4, x, mod);
    seq_t sum = r6;
    sum += r5;
    sum %= mod;
    sum += r4;
    sum %= mod;
    return sum == 0;
}

/* grow the cache array to hold at least required elements */
static void ensure_cache_size(size_t required)
{
    if (cache_cap >= required)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < required)
        newcap *= 2;
    seq_t *newptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (cache_len < n) {
        /* generate terms until we have the nth one */
        while (cache_len < n) {
            if (good(next_candidate)) {
                /* store it */
                ensure_cache_size(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
