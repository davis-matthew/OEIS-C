/*
 * A130058  Numbers m such that k = m*23^2 divides 3^(k-1) - 2^(k-1).
 * https://oeis.org/A130058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that (529*m) divides
 *            (3^(529*m‑1)-2^(529*m‑1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;      /* array of found m's (as seq_t) */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* capacity of 'cache' */
static idx_t last_checked = 0;  /* last m examined */

/* Grow the cache when needed. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* memory exhausted */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Test whether a given m satisfies the congruence. */
static bool good_m(idx_t m)
{
    /* k = 529 * m */
    seq_t k = (seq_t)m * (seq_t)529;
    /* exponent exp = k-1, stored as idx_t (fits for the intended n). */
    idx_t exp = (idx_t)(k - (seq_t)1);

    seq_t a3 = mod_pow((seq_t)3, exp, k);
    seq_t a2 = mod_pow((seq_t)2, exp, k);
    seq_t diff = a3 - a2;
    if (a3 < a2) diff += k;    /* adjust for unsigned underflow */

    return diff == 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough. */
    while (cache_len < n) {
        /* Find next m. */
        last_checked++;
        if (good_m(last_checked)) {
            /* Store m as seq_t. */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
