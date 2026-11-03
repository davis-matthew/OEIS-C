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
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th term of sequence A039587:
   numbers whose base‑6 representation has the same number of 1's,
   3's and 5's. It is implemented by a lazy brute–force search
   with a static cache of already found terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether a number has equal counts of 1, 3 and 5 in base 6. */
static bool good_base6(seq_t x)
{
    int c1 = 0, c3 = 0, c5 = 0;
    while (x > 0) {
        int d = (int)(x % 6);
        if (d == 1) c1++;
        else if (d == 3) c3++;
        else if (d == 5) c5++;
        x = x / 6;
    }
    return c1 == c3 && c3 == c5;
}

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
    seq_t *newp = mem_resize(seq_t, cache, new_cap);
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
