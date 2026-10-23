/*
 * A039335  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 7's.
 * https://oeis.org/A039335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   equal_zero_seven_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑8 representation has the same nonzero
 * number of 0's and 7's (OEIS A039335).  Implementation is a simple
 * brute‑force search that generates the terms in increasing order and
 * caches them for future calls.  The algorithm enumerates natural
 * numbers, converts each to base‑8, counts occurrences of digits 0 and
 * 7, and accepts the number when both counts are equal and positive. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_zero_seven_base8.h"

static seq_t *cache = NULL;      /* cached terms                     */
static size_t cache_len = 0;      /* how many terms are cached        */
static size_t cache_cap = 0;      /* current allocated capacity       */
static seq_t last = 0;            /* last candidate examined          */

/* Ensure the cache has space for at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* memory allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    if (cache_len >= (size_t)n)    /* already cached */
        return cache[n - 1];

    /* Expand cache to hold at least n terms */
    ensure_cache((size_t)n);

    seq_t candidate = last + 1;
    while (cache_len < (size_t)n) {
        if (equal_zero_seven_base8(candidate))
            cache[cache_len++] = candidate;
        candidate++;
    }
    last = candidate - 1;
    return cache[n - 1];
}
