/*
 * A037353  Numbers whose base-4 and base-6 expansions have no digits in
 *          common.
 * https://oeis.org/A037353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 540, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_common_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037353: numbers whose base‑4 and base‑6 expansions have no digits in common,
   computed by brute‑force search. */

#include <stdlib.h>
#include "../utils/has_common_digits.h"

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */
static seq_t last_candidate = 0; /* last integer tested */

/* Ensure the cache can hold at least 'need' terms. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* OEIS function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    while (cache_len < (size_t)n) {
        ++last_candidate;                   /* next integer to test */
        if (!has_common_digits(last_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_candidate;
        }
    }
    return cache[n - 1];
}
