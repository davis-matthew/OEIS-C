/*
 * A029799  Cubes k such that k^(2/3) and k have same digits.
 * https://oeis.org/A029799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A029797(n)^3, where A029797(n) is the n-th integer x
 *            whose square and cube contain the same set of decimal digits
 *            (ignoring multiplicity). This implementation searches x
 *            incrementally and memoises the sequence
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;       /* holds the found cubes */
static size_t cache_len = 0;      /* number of elements stored */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t next_x = 0;          /* next integer to test for property */

/* compute 2^x mask of decimal digits of x, 0->(1<<0) */
static unsigned digit_mask(seq_t x) {
    unsigned mask = 0;
    if (x == 0) return 1u << 0;
    while (x != 0) {
        unsigned d = (unsigned)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

/* grow the cache to at least 'need' elements */
static void ensure_cache(idx_t need) {
    if (cache_len >= (size_t)need) return;
    while (cache_len < (size_t)need) {
        seq_t x = (seq_t)next_x;
        seq_t sq = x * x;          /* x^2 */
        seq_t cb = sq * x;         /* x^3 */
        if (digit_mask(sq) == digit_mask(cb)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return; /* allocation failure, give up */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cb;
        }
        next_x++;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;      /* undefined for n<1, return 0 */
    ensure_cache(n);
    if (n <= cache_len) return cache[n-1];
    return 0; /* fallback */
}
