/*
 * A044861  Positive integers having the same number of base-5 runs of odd
 *          length as even.
 * https://oeis.org/A044861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Positive integers having the same number of base-5 runs of odd
 *            length as even
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Provides a(idx_t n) returning the nth term (1-indexed) of OEIS A044861. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache   = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static seq_t   next_candidate = 1;

/* Check whether the base‑5 representation of x has an equal number of
 * runs of odd length and runs of even length. */
static bool is_balanced(seq_t val)
{
    seq_t temp = val;
    /* Store digits least‑significant first; 32 is more than enough for 64‑bit values. */
    int digits[32];
    int len = 0;
    do {
        digits[len++] = (int)(temp % 5);
        temp = temp / 5;
    } while (temp > 0);

    int odd = 0, even = 0;
    int runlen = 1;
    for (int i = 1; i < len; ++i) {
        if (digits[i] == digits[i - 1]) {
            ++runlen;
        } else {
            if ((runlen & 1) == 0) ++even;
            else                    ++odd;
            runlen = 1;
        }
    }
    if ((runlen & 1) == 0) ++even;
    else                   ++odd;

    return odd == even;
}

/* Shrink or grow the cache array to hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; safety for n==0 */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        while (!is_balanced(next_candidate)) {
            ++next_candidate;
        }
        if (cache_len >= cache_cap) {
            grow_cache(cache_cap ? cache_cap * 2 : 8);
        }
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
