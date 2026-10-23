/*
 * A032567  Numbers k whose decimal representation, read as a base-18 value
 *          and divided by k, yields an integer.
 * https://oeis.org/A032567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A032567:  Numbers k whose decimal representation, read as a base‑18 value and divided by k, yields an integer. 
   The sequence is the ordered list of all such positive integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static bool good(idx_t k)
{
    /* Gather decimal digits of k (least significant first). */
    int digits[20];
    int len = 0;
    idx_t t = k;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }
    if (len == 0)
        return false;  /* k == 0 is not a candidate */

    /* Compute the base‑18 value modulo k. */
    idx_t mod = 0;
    for (int i = len - 1; i >= 0; --i) {
        mod = (mod * 18 + digits[i]) % k;
    }
    return mod == 0;
}

seq_t a(idx_t n)
{
    /* Static state to allow arbitrary call order and memoization. */
    static idx_t next_candidate = 1;
    static idx_t found_terms   = 0;
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;
    static size_t cache_size   = 0;

    /* Lazy allocation of cache on first call. */
    if (cache == NULL) {
        cache_cap  = 8;
        cache_size = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failure – minimal fallback */
    }

    /* Generate terms until we have at least n of them. */
    while (found_terms < n) {
        if (good(next_candidate)) {
            /* Grow cache if necessary. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;  /* allocation failure – minimal fallback */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
            ++found_terms;
        }
        ++next_candidate;
    }

    return cache[n - 1];  /* a(1) corresponds to cache[0] */
}
