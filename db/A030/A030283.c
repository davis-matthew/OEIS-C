/*
 * A030283  a(0) = 0; for n>0, a(n) is the smallest number greater than a(n-1)
 *          which does not use any digit used by a(n-1).
 * https://oeis.org/A030283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 78, u64 n <= 155,
 *            u128 n <= 309, big any n
 * Helpers:   uses_banned
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 10*a(n-8) - 10*a(n-10) for n > 29.
   First 30 terms (n = 0 … 29) are computed by brute force. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/uses_banned.h"

/* Static cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Expand cache capacity to contain at least up_to elements. */
static inline void ensure_cache(size_t up_to)
{
    if (up_to < cache_len)
        return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap <= up_to)  new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the next term given the previous term (brute force). */
static inline seq_t next_from(const seq_t prev)
{
    /* Compute digit mask of prev. */
    uint16_t banned = 0;
    seq_t v = prev;
    if (v == 0)
        banned = 1;              /* digit 0 */
    else {
        while (v > 0) {
            banned |= 1 << (int)(v % 10);
            v /= 10;
        }
    }

    seq_t cand = prev + 1;
    for (;;) {
        if (!uses_banned(cand, banned))
            return cand;
        cand++;
    }
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx + 1);

    if (n < cache_len)
        return cache[idx];

    if (cache_len == 0) {           /* first term a(0) */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    for (size_t i = cache_len; i <= idx; ++i) {
        if (i <= 29) {
            cache[i] = next_from(cache[i - 1]);
        } else {
            cache[i] = cache[i - 2] + 10 * cache[i - 8] - 10 * cache[i - 10];
        }
    }
    cache_len = idx + 1;
    return cache[idx];
}
