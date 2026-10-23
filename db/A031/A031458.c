/*
 * A031458  Numbers whose base-3 representation has 2 fewer 0's than 2's.
 * https://oeis.org/A031458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer whose base‑3 representation
   contains exactly two more 2’s than 0’s (i.e. #2 = #0 + 2). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_cand = 1;

/* Ensure the cache has at least capacity elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* True if x has two more 2’s than 0’s in its base‑3 expansion. */
static bool satisfies(seq_t x)
{
    seq_t t = x;
    unsigned int cnt0 = 0;
    unsigned int cnt2 = 0;
    while (t) {
        unsigned int d = (unsigned int)(t % 3);
        if (d == 0)
            ++cnt0;
        else if (d == 2)
            ++cnt2;
        t /= 3;
    }
    return cnt2 == cnt0 + 2;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined; return 0 for safety */

    size_t target = (size_t)n;
    while (cache_len < target) {
        if (satisfies(next_cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_cand;
        }
        ++next_cand;
    }
    return cache[target - 1];
}
