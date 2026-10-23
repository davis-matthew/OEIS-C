/*
 * A039587  Numbers whose base-6 representation has the same number of 1's,
 *          3's and 5's.
 * https://oeis.org/A039587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   good_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th term of sequence A039587:
   numbers whose base‑6 representation has the same number of 1's,
   3's and 5's. It is implemented by a lazy brute–force search
   with a static cache of already found terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_base6.h"

static seq_t *cache = NULL;      /* cache of found terms */
static size_t cache_count = 0;   /* number of terms stored */
static size_t cache_cap = 0;     /* capacity of cache */
static seq_t next_test = 0;      /* next integer to test */

/* Grow the cache to accommodate at least “need” elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* return the n-th term (n >= 1) */
    if (n < 1) return 0;  /* undefined, but avoid UB */

    /* generate terms on demand */
    while (cache_count < (size_t)n) {
        seq_t cand = next_test++;
        if (good_base6(cand)) {
            grow_cache(cache_count + 1);
            cache[cache_count++] = cand;
        }
    }
    return cache[n - 1];
}
